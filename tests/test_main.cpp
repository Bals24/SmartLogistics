// Jeu de tests minimal, sans dependance externe (pas de GoogleTest), pour
// verifier les briques les plus sensibles du projet. Chaque test affiche
// OK/ECHEC ; le programme renvoie un code non nul si un test a echoue,
// ce qui permet de l'utiliser tel quel dans un pipeline CI.

#include <iostream>
#include <cassert>
#include <cmath>

#include "entities/Commande.hpp"
#include "entities/Client.hpp"
#include "entities/Chauffeur.hpp"
#include "factories/VehiculeFactory.hpp"
#include "factories/ColisFactory.hpp"
#include "graphe/Graphe.hpp"
#include "graphe/Dijkstra.hpp"
#include "strategies/StrategiesConcretes.hpp"
#include "services/AllocateurRessources.hpp"
#include "persistence/PersistanceJSON.hpp"
#include "utils/Exceptions.hpp"

using namespace sl;

static int nbEchecs = 0;

void verifier(bool condition, const std::string& nomTest) {
    if (condition) {
        std::cout << "[OK]    " << nomTest << "\n";
    } else {
        std::cout << "[ECHEC] " << nomTest << "\n";
        ++nbEchecs;
    }
}

void testCycleDeVieCommande() {
    Client client("CLT-T1", "Client Test", "000", "Antananarivo");
    Commande commande("ORD-T1", &client, "Toamasina");

    verifier(commande.getStatut() == StatutCommande::CREEE, "Commande demarre a CREEE");

    commande.avancer();
    verifier(commande.getStatut() == StatutCommande::CONFIRMEE, "avancer() -> CONFIRMEE");

    bool exceptionLevee = false;
    try {
        commande.avancer();
        commande.avancer();
        commande.avancer();
        commande.avancer(); // -> EN_TRANSIT
        commande.annulerCommande(); // doit echouer : trop tard pour annuler
    } catch (const TransitionInvalideException&) {
        exceptionLevee = true;
    }
    verifier(exceptionLevee, "annulerCommande() rejetee depuis EN_TRANSIT");
}

void testFactoryVehicule() {
    auto camion = VehiculeFactory::creer("CAMION", {"C-T1", "Marque", "Modele", 3000.0});
    verifier(camion->getType() == "CAMION", "VehiculeFactory cree un CAMION");
    verifier(camion->permisRequis() == TypePermis::C, "Un camion exige le permis C");

    bool exceptionLevee = false;
    try {
        VehiculeFactory::creer("INCONNU", {"X", "X", "X", 100.0});
    } catch (const TypeInconnuException&) {
        exceptionLevee = true;
    }
    verifier(exceptionLevee, "VehiculeFactory rejette un type inconnu");
}

void testColisRefrigereExigeVehiculeSpecial() {
    auto colis = ColisFactory::creer("REFRIGERE",
        {"PKG-T1", "Test", 1.0, 0.01, "A", "B", Priorite::STANDARD, {}, {}, 2.0, 8.0});
    verifier(colis->necessiteVehiculeSpecial(), "Un colis REFRIGERE exige un vehicule special");
}

void testDijkstraPlusCourtChemin() {
    Graphe graphe;
    graphe.ajouterRoute(Route("A", "B", 10.0, 1.0, 100.0));
    graphe.ajouterRoute(Route("B", "C", 10.0, 1.0, 100.0));
    graphe.ajouterRoute(Route("A", "C", 30.0, 1.0, 50.0)); // plus court en distance directe mais plus long

    StrategiePlusCourtChemin strategie;
    Itineraire itineraire = calculerPlusCourtChemin(graphe, "A", "C", strategie);

    verifier(itineraire.trouve, "Un chemin est trouve entre A et C");
    verifier(std::abs(itineraire.distanceTotaleKm - 20.0) < 0.001,
             "Le plus court chemin passe par B (20 km, pas 30 km directs)");
}

void testDijkstraIgnoreRouteBloquee() {
    Graphe graphe;
    graphe.ajouterRoute(Route("A", "B", 10.0, 1.0, 100.0, EtatRoute::BLOQUEE));
    graphe.ajouterRoute(Route("A", "C", 5.0, 1.0, 50.0));
    graphe.ajouterRoute(Route("C", "B", 5.0, 1.0, 50.0));

    StrategiePlusCourtChemin strategie;
    Itineraire itineraire = calculerPlusCourtChemin(graphe, "A", "B", strategie);

    verifier(itineraire.trouve, "Un detour est trouve malgre la route bloquee");
    verifier(std::abs(itineraire.distanceTotaleKm - 10.0) < 0.001,
             "Le detour A->C->B (10 km) est prefere a la route bloquee A->B");
}

void testAllocateurRessourcesRejetteSiIncompatible() {
    Client client("CLT-T2", "Client Test", "000", "Antananarivo");
    Commande commande("ORD-T2", &client, "Toamasina");
    commande.ajouterColis(ColisFactory::creer("REFRIGERE",
        {"PKG-T2", "Vaccins", 5.0, 0.01, "A", "B", Priorite::URGENTE, {}, {}, 2.0, 8.0}));

    auto camion = VehiculeFactory::creer("CAMION", {"C-T2", "M", "M", 3000.0});
    std::vector<Vehicule*> vehicules{camion.get()};

    Chauffeur chauffeur("DRV-T2", "Test", TypePermis::C, 5);
    std::vector<Chauffeur*> chauffeurs{&chauffeur};

    ResultatAffectation resultat = AllocateurRessources::affecter(commande, vehicules, chauffeurs);
    verifier(!resultat.reussie, "Un camion non-frigorifique est rejete pour un colis REFRIGERE");
}

void testAllocateurChoisitLeVehiculeAdapteMemeGrandeCapacite() {
    // Regression : l'ancien score (1000 - gaspillage) devenait negatif pour les
    // vehicules de grande capacite, et plus aucun vehicule n'etait choisi.
    Client client("CLT-T4", "Client Test", "000", "Antananarivo");
    Commande commande("ORD-T4", &client, "Toamasina");
    commande.ajouterColis(ColisFactory::creer("STANDARD",
        {"PKG-T4", "Colis leger", 100.0, 0.5, "A", "B", Priorite::STANDARD, {}, {}, {}, {}}));

    auto petit = VehiculeFactory::creer("FOURGON", {"F-T4", "M", "M", 2000.0, 0.0, 80.0});
    auto grand = VehiculeFactory::creer("CAMION",  {"C-T4", "M", "M", 5000.0, 0.0, 80.0});
    std::vector<Vehicule*> vehicules{grand.get(), petit.get()};

    Chauffeur conducteurB("DRV-T4B", "Leger", TypePermis::B, 4);
    Chauffeur conducteurC("DRV-T4C", "Lourd", TypePermis::C, 9);
    std::vector<Chauffeur*> chauffeurs{&conducteurB, &conducteurC};

    ResultatAffectation resultat = AllocateurRessources::affecter(commande, vehicules, chauffeurs);
    verifier(resultat.reussie, "Un vehicule de 2000 kg est trouve pour un colis de 100 kg");
    verifier(resultat.vehicule == petit.get(),
             "Le vehicule le plus adapte (le plus petit suffisant) est prefere au camion");
    verifier(resultat.score > 0.0, "Le score d'un vehicule choisi est positif");
}

void testPersistanceJSONRoundTrip() {
    Client original("CLT-T3", "Test JSON", "011", "Fianarantsoa");
    auto j = PersistanceJSON::versJson(original);
    Client reconstruit = PersistanceJSON::clientDepuisJson(j);
    verifier(reconstruit.getId() == original.getId() && reconstruit.getNom() == original.getNom(),
             "Client : round-trip JSON identique");

    auto frigo = VehiculeFactory::creer("FRIGORIFIQUE", {"R-T3", "M", "M", 2000.0, 0.0, 100.0, -18.0});
    auto jv = PersistanceJSON::versJson(*frigo);
    auto reconstruitV = PersistanceJSON::vehiculeDepuisJson(jv);
    verifier(reconstruitV->estFrigorifique(), "Vehicule frigorifique : le type survit au round-trip JSON");
}

int main() {
    std::cout << "=== Tests Smart Logistics ===\n";
    testCycleDeVieCommande();
    testFactoryVehicule();
    testColisRefrigereExigeVehiculeSpecial();
    testDijkstraPlusCourtChemin();
    testDijkstraIgnoreRouteBloquee();
    testAllocateurRessourcesRejetteSiIncompatible();
    testAllocateurChoisitLeVehiculeAdapteMemeGrandeCapacite();
    testPersistanceJSONRoundTrip();

    std::cout << "\n";
    if (nbEchecs == 0) {
        std::cout << "Tous les tests sont passes.\n";
        return 0;
    }
    std::cout << nbEchecs << " test(s) en echec.\n";
    return 1;
}