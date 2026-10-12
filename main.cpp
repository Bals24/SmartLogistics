// Smart Logistics & Fleet Management
// Point d'entree : menu console (Niveau 7) qui relie entre elles toutes les
// briques construites aux niveaux precedents : entites (Niveau 1),
// logistique/etats (Niveau 2), graphe + strategies d'itineraire (Niveau 3),
// persistance JSON (Niveau 4), Factory/Observer/State/Repository (Niveau 5),
// surveillance de flotte en arriere-plan (Niveau 6).

#include <iostream>
#include <sstream>
#include <limits>
#include <filesystem>
#include <chrono>

#include "entities/Client.hpp"
#include "entities/Chauffeur.hpp"
#include "entities/Commande.hpp"
#include "entities/IncidentsConcrets.hpp"
#include "factories/VehiculeFactory.hpp"
#include "factories/ColisFactory.hpp"
#include "repositories/Repositories.hpp"
#include "graphe/Graphe.hpp"
#include "graphe/Dijkstra.hpp"
#include "graphe/ReseauRoutier.hpp"
#include "strategies/StrategiesConcretes.hpp"
#include "observateurs/ObservateursConcrets.hpp"
#include "services/GestionnaireFlotte.hpp"
#include "services/ServiceMaintenance.hpp"
#include "services/AllocateurRessources.hpp"
#include "services/ServiceStatistiques.hpp"
#include "services/SurveillanceFlotte.hpp"
#include "persistence/PersistanceJSON.hpp"

#ifdef SL_HAS_POSTGRES
#include "database/Database.hpp"
#include "database/PersistancePostgres.hpp"
#endif
#include "utils/IdGenerator.hpp"
#include "utils/Exceptions.hpp"

using namespace sl;
namespace fs = std::filesystem;

// ============================================================================
// Etat global de l'application (regroupe pour eviter une longue liste de
// parametres a chaque fonction de menu).
// ============================================================================
struct ApplicationSmartLogistics {
    ClientRepository clients;
    ChauffeurRepository chauffeurs;
    GestionnaireFlotte flotte;
    CommandeRepository commandes;
    LivraisonRepository livraisons;
    Graphe graphe;
    ServiceMaintenance serviceMaintenance;

    NotificationClient observateurClient;
    NotificationGestionnaire observateurGestionnaire;

    const std::string villeDepot = "Antananarivo";
};

// ============================================================================
// Petits utilitaires d'entree/sortie console
// ============================================================================

int lireEntier(const std::string& invite, int min, int max) {
    while (true) {
        std::cout << invite;
        std::string ligne;
        std::getline(std::cin, ligne);
        try {
            std::size_t position = 0;
            int valeur = std::stoi(ligne, &position);
            if (position == ligne.size() && valeur >= min && valeur <= max) {
                return valeur;
            }
        } catch (...) {
            // ignore, redemande
        }
        std::cout << "  -> Entree invalide, recommencez (" << min << "-" << max << ").\n";
    }
}

double lireDouble(const std::string& invite, double min = 0.0) {
    while (true) {
        std::cout << invite;
        std::string ligne;
        std::getline(std::cin, ligne);
        try {
            std::size_t position = 0;
            double valeur = std::stod(ligne, &position);
            if (position == ligne.size() && valeur >= min) return valeur;
        } catch (...) {
        }
        std::cout << "  -> Nombre invalide, recommencez.\n";
    }
}

std::string lireLigne(const std::string& invite) {
    std::cout << invite;
    std::string ligne;
    std::getline(std::cin, ligne);
    return ligne;
}

void pauseConsole() {
    std::cout << "\n(Appuyez sur Entree pour continuer)";
    std::string ignore;
    std::getline(std::cin, ignore);
}

// ============================================================================
// Initialisation des donnees de demonstration
// ============================================================================

void construireGrapheVilles(Graphe& graphe) {
    // Le reseau routier est partage avec l'interface graphique
    // (graphe/ReseauRoutier.cpp).
    construireReseauMalgache(graphe);
}

void initialiserDonneesDemo(ApplicationSmartLogistics& app) {
    // --- Clients ---
    app.clients.ajouter(std::make_unique<Client>(
        "CLT-001", "Entreprise X", "034 00 000 01", "Antananarivo"));
    app.clients.ajouter(std::make_unique<Client>(
        "CLT-002", "Rakoto Import", "033 00 000 02", "Antananarivo"));

    // --- Chauffeurs ---
    app.chauffeurs.ajouter(std::make_unique<Chauffeur>("DRV-001", "Rakoto", TypePermis::C, 7));
    app.chauffeurs.ajouter(std::make_unique<Chauffeur>("DRV-002", "Rasoa", TypePermis::B, 3));
    app.chauffeurs.ajouter(std::make_unique<Chauffeur>("DRV-003", "Naina", TypePermis::MOTO, 2));
    app.chauffeurs.ajouter(std::make_unique<Chauffeur>("DRV-004", "Voahangy", TypePermis::C1, 5));

    // --- Flotte (via la Factory, Niveau 5) ---
    app.flotte.ajouterVehicule(VehiculeFactory::creer("CAMION",
        {"C-001", "Isuzu", "FVR", 5000.0, 126000.0, 72.0}));
    app.flotte.ajouterVehicule(VehiculeFactory::creer("FOURGON",
        {"F-001", "Renault", "Master", 1200.0, 58000.0, 90.0}));
    app.flotte.ajouterVehicule(VehiculeFactory::creer("MOTO",
        {"M-001", "Yamaha", "YBR125", 40.0, 12000.0, 100.0}));
    ParametresVehicule paramsFrigo{"R-001", "Mercedes", "Actros", 4000.0, 95000.0, 60.0, 4.0};
    app.flotte.ajouterVehicule(VehiculeFactory::creer("FRIGORIFIQUE", paramsFrigo));

    // --- Historique de maintenance ---
    // C-001 a depasse le seuil (126 000 km > 118 000 + 5 000) : sera detecte
    // des la premiere verification, comme dans l'exemple du cahier des charges.
    app.serviceMaintenance.enregistrerMaintenance(std::make_unique<Maintenance>(
        "MTN-0001", "C-001", TypeMaintenance::VIDANGE, 118000.0, 5000.0,
        "Derniere vidange avant depassement du seuil"));

    // --- Graphe routier ---
    construireGrapheVilles(app.graphe);

    // --- Une commande de demonstration deja prete a l'emploi ---
    Client* client = app.clients.trouverParId("CLT-001");
    auto commande = std::make_unique<Commande>("ORD-1024", client, "Toamasina", Priorite::EXPRESS);
    commande->abonner(&app.observateurClient);
    commande->abonner(&app.observateurGestionnaire);
    commande->ajouterColis(ColisFactory::creer("STANDARD",
        {"PKG-001", "Fournitures de bureau", 12.5, 0.05, app.villeDepot, "Toamasina",
         Priorite::STANDARD, {}, {}, {}, {}}));
    ParametresColis paramsFragile;
    paramsFragile.id = "PKG-002"; paramsFragile.description = "Vaisselle en verre";
    paramsFragile.poidsKg = 8.0; paramsFragile.volumeM3 = 0.04;
    paramsFragile.origine = app.villeDepot; paramsFragile.destination = "Toamasina";
    paramsFragile.consigneManutention = "Ne pas empiler";
    commande->ajouterColis(ColisFactory::creer("FRAGILE", paramsFragile));
    app.commandes.ajouter(std::move(commande));
}

// ============================================================================
// Actions du menu
// ============================================================================

void listerClients(ApplicationSmartLogistics& app) {
    std::cout << "\n--- CLIENTS (" << app.clients.taille() << ") ---\n";
    for (const Client* c : app.clients.tousLesElements()) c->afficherInformations();
}

void listerChauffeurs(ApplicationSmartLogistics& app) {
    std::cout << "\n--- CHAUFFEURS (" << app.chauffeurs.taille() << ") ---\n";
    for (const Chauffeur* c : app.chauffeurs.tousLesElements()) c->afficherInformations();
}

void listerFlotte(ApplicationSmartLogistics& app) {
    auto vehicules = app.flotte.tousLesVehicules();
    std::cout << "\n--- FLOTTE (" << vehicules.size() << ") ---\n";
    for (Vehicule* v : vehicules) {
        v->afficherInformations();
        std::cout << "  Permis requis : " << toString(v->permisRequis()) << "\n";
    }
}

void listerCommandes(ApplicationSmartLogistics& app) {
    auto liste = app.commandes.tousLesElements();
    std::cout << "\n--- COMMANDES (" << liste.size() << ") ---\n";
    for (Commande* c : liste) c->afficherInformations();
}

void listerLivraisons(ApplicationSmartLogistics& app) {
    auto liste = app.livraisons.tousLesElements();
    std::cout << "\n--- LIVRAISONS (" << liste.size() << ") ---\n";
    for (Livraison* l : liste) l->afficherInformations();
}

Commande* choisirCommande(ApplicationSmartLogistics& app) {
    auto liste = app.commandes.tousLesElements();
    if (liste.empty()) {
        std::cout << "Aucune commande enregistree.\n";
        return nullptr;
    }
    std::cout << "\nCommandes disponibles :\n";
    for (std::size_t i = 0; i < liste.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << liste[i]->getId()
                   << " [" << liste[i]->getNomEtat() << "] -> " << liste[i]->getDestination() << "\n";
    }
    int choix = lireEntier("Choix : ", 1, static_cast<int>(liste.size()));
    return liste[choix - 1];
}

void creerCommande(ApplicationSmartLogistics& app) {
    auto clientsListe = app.clients.tousLesElements();
    if (clientsListe.empty()) { std::cout << "Aucun client enregistre.\n"; return; }

    std::cout << "\nClients :\n";
    for (std::size_t i = 0; i < clientsListe.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << clientsListe[i]->getNom() << "\n";
    }
    int choixClient = lireEntier("Client : ", 1, static_cast<int>(clientsListe.size()));
    Client* client = clientsListe[choixClient - 1];

    std::string destination = lireLigne("Ville de destination : ");
    std::cout << "Priorite : 1.STANDARD  2.EXPRESS  3.URGENTE\n";
    int choixPriorite = lireEntier("Choix : ", 1, 3);
    Priorite priorite = (choixPriorite == 1) ? Priorite::STANDARD
                       : (choixPriorite == 2) ? Priorite::EXPRESS : Priorite::URGENTE;

    std::string idCommande = IdGenerator::suivant("ORD");
    auto commande = std::make_unique<Commande>(idCommande, client, destination, priorite);
    commande->abonner(&app.observateurClient);
    commande->abonner(&app.observateurGestionnaire);

    int nbColis = lireEntier("Combien de colis a ajouter (0-5) ? ", 0, 5);
    for (int i = 0; i < nbColis; ++i) {
        std::cout << "\nColis " << (i + 1) << " - type : "
                   << "1.STANDARD  2.FRAGILE  3.EXPRESS  4.REFRIGERE\n";
        int typeChoisi = lireEntier("Choix : ", 1, 4);
        ParametresColis params;
        params.id = IdGenerator::suivant("PKG");
        params.description = lireLigne("  Description : ");
        params.poidsKg = lireDouble("  Poids (kg) : ", 0.01);
        params.volumeM3 = lireDouble("  Volume (m3) : ", 0.001);
        params.origine = app.villeDepot;
        params.destination = destination;

        std::string type;
        if (typeChoisi == 1) {
            type = "STANDARD";
        } else if (typeChoisi == 2) {
            type = "FRAGILE";
            params.consigneManutention = lireLigne("  Consigne de manutention : ");
        } else if (typeChoisi == 3) {
            type = "EXPRESS";
            params.delaiMaxHeures = lireEntier("  Delai maximal (heures) : ", 1, 240);
        } else {
            type = "REFRIGERE";
            params.temperatureMinC = lireDouble("  Temperature min (C) : ", -30.0);
            params.temperatureMaxC = lireDouble("  Temperature max (C) : ", -30.0);
        }
        commande->ajouterColis(ColisFactory::creer(type, params));
    }

    std::cout << "\nCommande " << idCommande << " creee avec succes.\n";
    commande->afficherInformations();
    app.commandes.ajouter(std::move(commande));
}

void avancerCommande(ApplicationSmartLogistics& app) {
    Commande* commande = choisirCommande(app);
    if (!commande) return;
    try {
        commande->avancer();
        std::cout << "Nouvel etat de " << commande->getId() << " : " << commande->getNomEtat() << "\n";
    } catch (const TransitionInvalideException& e) {
        std::cout << "Impossible : " << e.what() << "\n";
    }
}

void annulerCommande(ApplicationSmartLogistics& app) {
    Commande* commande = choisirCommande(app);
    if (!commande) return;
    try {
        commande->annulerCommande();
        std::cout << "Commande " << commande->getId() << " annulee.\n";
    } catch (const TransitionInvalideException& e) {
        std::cout << "Impossible : " << e.what() << "\n";
    }
}

const StrategieItineraire* choisirStrategie(const StrategiePlusCourtChemin& s1,
                                             const StrategiePlusRapide& s2,
                                             const StrategieMoinsCher& s3) {
    std::cout << "Strategie d'itineraire : 1.Plus court  2.Plus rapide  3.Moins cher\n";
    int choix = lireEntier("Choix : ", 1, 3);
    if (choix == 1) return &s1;
    if (choix == 2) return &s2;
    return &s3;
}

void affecterRessources(ApplicationSmartLogistics& app) {
    Commande* commande = choisirCommande(app);
    if (!commande) return;

    if (commande->getStatut() != StatutCommande::COLIS_PRET) {
        std::cout << "Cette commande doit d'abord atteindre l'etat COLIS_PRET "
                   << "(etat actuel : " << commande->getNomEtat() << "). "
                   << "Utilisez \"Faire avancer une commande\".\n";
        return;
    }

    auto vehiculesDisponibles = app.flotte.vehiculesDisponibles();
    std::vector<Chauffeur*> chauffeursDisponibles;
    for (Chauffeur* c : app.chauffeurs.tousLesElements()) {
        if (c->getStatut() == StatutChauffeur::DISPONIBLE) chauffeursDisponibles.push_back(c);
    }

    ResultatAffectation resultat = AllocateurRessources::affecter(
        *commande, vehiculesDisponibles, chauffeursDisponibles);

    if (!resultat.reussie) {
        std::cout << "Affectation impossible : " << resultat.raisonEchec << "\n";
        return;
    }

    StrategiePlusCourtChemin strategieCourte;
    StrategiePlusRapide strategieRapide;
    StrategieMoinsCher strategieEconomique;
    const StrategieItineraire* strategie =
        choisirStrategie(strategieCourte, strategieRapide, strategieEconomique);

    Itineraire itineraire = calculerPlusCourtChemin(
        app.graphe, app.villeDepot, commande->getDestination(), *strategie);

    if (!itineraire.trouve) {
        std::cout << "Aucun itineraire trouve entre " << app.villeDepot
                   << " et " << commande->getDestination() << ".\n";
        return;
    }

    resultat.vehicule->setStatut(StatutVehicule::EN_MISSION);
    resultat.chauffeur->setStatut(StatutChauffeur::EN_MISSION);

    std::string idLivraison = IdGenerator::suivant("DLV");
    auto livraison = std::make_unique<Livraison>(
        idLivraison, commande, resultat.vehicule, resultat.chauffeur, itineraire);

    std::cout << "\nVehicule retenu  : " << resultat.vehicule->getImmatriculation()
               << " (score " << resultat.score << ")\n";
    std::cout << "Chauffeur retenu : " << resultat.chauffeur->getNom() << "\n";
    itineraire.afficherInformations();

    // Progression naturelle de la commande maintenant que les ressources sont affectees.
    try {
        commande->avancer(); // COLIS_PRET -> AFFECTATION_VEHICULE
        commande->avancer(); // AFFECTATION_VEHICULE -> EXPEDITION
    } catch (const TransitionInvalideException&) {
        // deja plus loin dans le cycle, rien a faire
    }

    app.livraisons.ajouter(std::move(livraison));
    std::cout << "Livraison " << idLivraison << " creee.\n";
}

void calculerItineraireSeul(ApplicationSmartLogistics& app) {
    std::cout << "Villes disponibles : ";
    for (const auto& ville : app.graphe.getVilles()) std::cout << ville << "  ";
    std::cout << "\n";

    std::string depart = lireLigne("Ville de depart : ");
    std::string arrivee = lireLigne("Ville d'arrivee : ");

    if (!app.graphe.contientVille(depart) || !app.graphe.contientVille(arrivee)) {
        std::cout << "Ville inconnue dans le graphe.\n";
        return;
    }

    StrategiePlusCourtChemin strategieCourte;
    StrategiePlusRapide strategieRapide;
    StrategieMoinsCher strategieEconomique;
    const StrategieItineraire* strategie =
        choisirStrategie(strategieCourte, strategieRapide, strategieEconomique);

    Itineraire itineraire = calculerPlusCourtChemin(app.graphe, depart, arrivee, *strategie);
    itineraire.afficherInformations();
}

void declarerIncident(ApplicationSmartLogistics& app) {
    auto liste = app.livraisons.tousLesElements();
    if (liste.empty()) { std::cout << "Aucune livraison enregistree.\n"; return; }

    std::cout << "\nLivraisons :\n";
    for (std::size_t i = 0; i < liste.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << liste[i]->getId() << "\n";
    }
    int choixLivraison = lireEntier("Choix : ", 1, static_cast<int>(liste.size()));
    Livraison* livraison = liste[choixLivraison - 1];

    std::cout << "Type d'incident : 1.Accident  2.Panne  3.Route bloquee  4.Retard  5.Colis endommage\n";
    int type = lireEntier("Choix : ", 1, 5);
    std::string description = lireLigne("Description : ");
    std::string localisation = lireLigne("Localisation : ");
    std::string idIncident = IdGenerator::suivant("INC");

    std::unique_ptr<Incident> incident;
    if (type == 1) {
        incident = std::make_unique<Accident>(idIncident, description, localisation, livraison->getId());
    } else if (type == 2) {
        std::string typePanne = lireLigne("Type de panne (mecanique/carburant/pneu...) : ");
        incident = std::make_unique<Panne>(idIncident, description, localisation, livraison->getId(), typePanne);
    } else if (type == 3) {
        incident = std::make_unique<RouteBloquee>(idIncident, description, localisation, livraison->getId());
    } else if (type == 4) {
        int minutes = lireEntier("Retard estime (minutes) : ", 1, 1440);
        incident = std::make_unique<Retard>(idIncident, description, localisation, livraison->getId(), minutes);
    } else {
        std::string colisId = lireLigne("Identifiant du colis concerne : ");
        incident = std::make_unique<ColisEndommage>(idIncident, description, localisation, livraison->getId(), colisId);
    }

    bool bloquant = incident->bloqueLivraison();
    incident->afficherInformations();
    livraison->ajouterIncident(std::move(incident));

    if (bloquant) {
        livraison->setStatut(StatutLivraison::RETARDEE);
        if (livraison->getCommande()) {
            try {
                livraison->getCommande()->declarerIncident();
            } catch (const TransitionInvalideException& e) {
                std::cout << "(Commande deja en incident ou terminale : " << e.what() << ")\n";
            }
        }
    }
    std::cout << "Incident " << idIncident << " enregistre sur la livraison " << livraison->getId() << ".\n";
}

void verifierMaintenance(ApplicationSmartLogistics& app) {
    auto aTraiter = app.serviceMaintenance.detecterEtMarquer(app.flotte);
    if (aTraiter.empty()) {
        std::cout << "Aucune maintenance requise pour le moment.\n";
        return;
    }
    std::cout << "/!\\ Vehicules necessitant une maintenance :\n";
    for (Vehicule* v : aTraiter) {
        std::cout << "  - " << v->getImmatriculation() << " (" << v->getKilometrage() << " km)\n";
    }
}

void afficherStatistiques(ApplicationSmartLogistics& app) {
    auto rapport = ServiceStatistiques::genererRapport(app.livraisons.tousLesElements(), app.flotte);
    ServiceStatistiques::afficherRapport(rapport);
}

void sauvegarderDonnees(ApplicationSmartLogistics& app) {
    try {
        fs::create_directories("data");
        PersistanceJSON::sauvegarderClients("data/clients.json", app.clients);
        PersistanceJSON::sauvegarderChauffeurs("data/chauffeurs.json", app.chauffeurs);
        PersistanceJSON::sauvegarderFlotte("data/flotte.json", app.flotte);
        std::cout << "Donnees sauvegardees dans le dossier data/.\n";
    } catch (const PersistanceException& e) {
        std::cout << "Erreur de sauvegarde : " << e.what() << "\n";
    }
}

void chargerDonnees(ApplicationSmartLogistics& app) {
    try {
        PersistanceJSON::chargerClients("data/clients.json", app.clients);
        PersistanceJSON::chargerChauffeurs("data/chauffeurs.json", app.chauffeurs);
        PersistanceJSON::chargerFlotte("data/flotte.json", app.flotte);
        std::cout << "Donnees rechargees depuis le dossier data/ "
                   << "(ajoutees a celles deja en memoire).\n";
    } catch (const PersistanceException& e) {
        std::cout << "Erreur de chargement : " << e.what()
                   << " (avez-vous fait une sauvegarde au prealable ?)\n";
    }
}

void sauvegarderPostgres(ApplicationSmartLogistics& app) {
#ifdef SL_HAS_POSTGRES
    SmartLogistics::Database db;
    if (!db.connect()) {
        std::cout << "Connexion PostgreSQL impossible : " << db.lastError() << "\n"
                  << "(verifiez que le serveur tourne et que la variable "
                  << "SL_DB_PASSWORD est definie)\n";
        return;
    }
    try {
        PersistancePostgres::sauvegarderTout(db, app.clients, app.chauffeurs, app.flotte);
        std::cout << "Clients, chauffeurs et flotte sauvegardes dans PostgreSQL.\n";
    } catch (const SmartLogisticsException& e) {
        std::cout << "Erreur de sauvegarde : " << e.what() << "\n";
    }
#else
    (void)app;
    std::cout << "PostgreSQL n'est pas disponible dans cette compilation.\n";
#endif
}

void chargerPostgres(ApplicationSmartLogistics& app) {
#ifdef SL_HAS_POSTGRES
    SmartLogistics::Database db;
    if (!db.connect()) {
        std::cout << "Connexion PostgreSQL impossible : " << db.lastError() << "\n"
                  << "(verifiez que le serveur tourne et que la variable "
                  << "SL_DB_PASSWORD est definie)\n";
        return;
    }
    try {
        ResumeChargement resume = PersistancePostgres::chargerTout(
            db, app.clients, app.chauffeurs, app.flotte);
        std::cout << "Chargement PostgreSQL : " << resume.ajoutes << " element(s) ajoute(s), "
                  << resume.ignores << " ignore(s) (deja en memoire ou type inconnu).\n";
    } catch (const SmartLogisticsException& e) {
        std::cout << "Erreur de chargement : " << e.what() << "\n";
    }
#else
    (void)app;
    std::cout << "PostgreSQL n'est pas disponible dans cette compilation.\n";
#endif
}

void afficherMenu() {
    std::cout << "\n===================================\n";
    std::cout << "      SMART LOGISTICS\n";
    std::cout << "===================================\n";
    std::cout << " 1. Lister les clients\n";
    std::cout << " 2. Lister les chauffeurs\n";
    std::cout << " 3. Lister la flotte de vehicules\n";
    std::cout << " 4. Lister les commandes\n";
    std::cout << " 5. Creer une nouvelle commande\n";
    std::cout << " 6. Faire avancer une commande (etape suivante)\n";
    std::cout << " 7. Annuler une commande\n";
    std::cout << " 8. Affecter vehicule + chauffeur (auto) et calculer l'itineraire\n";
    std::cout << " 9. Calculer un itineraire (sans creer de livraison)\n";
    std::cout << "10. Lister les livraisons\n";
    std::cout << "11. Declarer un incident sur une livraison\n";
    std::cout << "12. Verifier les vehicules a entretenir\n";
    std::cout << "13. Afficher les statistiques\n";
    std::cout << "14. Sauvegarder (clients, chauffeurs, flotte) en JSON\n";
    std::cout << "15. Charger des donnees JSON (data/)\n";
    std::cout << "16. Sauvegarder (clients, chauffeurs, flotte) dans PostgreSQL\n";
    std::cout << "17. Charger depuis PostgreSQL\n";
    std::cout << " 0. Quitter\n";
}

int main() {
    ApplicationSmartLogistics app;
    initialiserDonneesDemo(app);

    // Niveau 6 - un thread verifie la flotte toutes les 5 secondes en tache
    // de fond, independamment du menu console qui tourne sur le thread principal.
    SurveillanceFlotte surveillance(app.flotte, app.serviceMaintenance, std::chrono::milliseconds(5000));
    surveillance.demarrer();

    std::cout << "Bienvenue dans Smart Logistics & Fleet Management.\n";
    std::cout << "(Une surveillance de flotte tourne en arriere-plan toutes les 5s.)\n";

    bool continuer = true;
    while (continuer) {
        afficherMenu();
        int choix = lireEntier("Choix : ", 0, 17);
        switch (choix) {
            case 1:  listerClients(app); break;
            case 2:  listerChauffeurs(app); break;
            case 3:  listerFlotte(app); break;
            case 4:  listerCommandes(app); break;
            case 5:  creerCommande(app); break;
            case 6:  avancerCommande(app); break;
            case 7:  annulerCommande(app); break;
            case 8:  affecterRessources(app); break;
            case 9:  calculerItineraireSeul(app); break;
            case 10: listerLivraisons(app); break;
            case 11: declarerIncident(app); break;
            case 12: verifierMaintenance(app); break;
            case 13: afficherStatistiques(app); break;
            case 14: sauvegarderDonnees(app); break;
            case 15: chargerDonnees(app); break;
            case 16: sauvegarderPostgres(app); break;
            case 17: chargerPostgres(app); break;
            case 0:  continuer = false; break;
        }
        if (continuer) pauseConsole();
    }

    surveillance.arreter();
    std::cout << "\nA bientot !\n";
    return 0;
}