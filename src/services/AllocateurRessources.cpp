#include "services/AllocateurRessources.hpp"

#include <limits>

namespace sl {

double AllocateurRessources::calculerScore(const Vehicule& vehicule, double poidsCommandeKg) {
    // Taux de remplissage du vehicule (0 a 100) : plus la commande "remplit"
    // le vehicule, moins on gaspille de capacite. Contrairement a l'ancienne
    // formule (1000 - gaspillage), ce score ne devient jamais negatif, quelle
    // que soit la capacite du vehicule (camion de 5 T compris).
    const double capacite = vehicule.getCapaciteKg();
    const double scoreCapacite = capacite > 0.0 ? (poidsCommandeKg / capacite) * 100.0 : 0.0;
    const double scoreCarburant = vehicule.getNiveauCarburantPct();   // 0 a 100
    return scoreCapacite * 0.7 + scoreCarburant * 0.3;
}

ResultatAffectation AllocateurRessources::affecter(
        const Commande& commande,
        const std::vector<Vehicule*>& vehiculesDisponibles,
        const std::vector<Chauffeur*>& chauffeursDisponibles) {

    ResultatAffectation resultat;
    const double poids = commande.poidsTotalKg();
    const bool besoinFrigo = commande.necessiteVehiculeSpecial();

    Vehicule* meilleurVehicule = nullptr;
    double meilleurScore = std::numeric_limits<double>::lowest();

    for (Vehicule* v : vehiculesDisponibles) {
        if (v->getStatut() != StatutVehicule::DISPONIBLE) continue;
        if (v->getCapaciteKg() < poids) continue;
        if (besoinFrigo && !v->estFrigorifique()) continue;

        double score = calculerScore(*v, poids);
        if (score > meilleurScore) {
            meilleurScore = score;
            meilleurVehicule = v;
        }
    }

    if (!meilleurVehicule) {
        resultat.raisonEchec = besoinFrigo
            ? "Aucun vehicule frigorifique disponible avec une capacite suffisante"
            : "Aucun vehicule disponible n'a une capacite suffisante (" +
              std::to_string(static_cast<long>(poids)) + " kg requis)";
        return resultat;
    }

    Chauffeur* meilleurChauffeur = nullptr;
    int meilleureExperience = -1;

    for (Chauffeur* c : chauffeursDisponibles) {
        if (c->getStatut() != StatutChauffeur::DISPONIBLE) continue;
        if (!c->peutConduire(meilleurVehicule->permisRequis())) continue;
        if (c->getExperienceAnnees() > meilleureExperience) {
            meilleureExperience = c->getExperienceAnnees();
            meilleurChauffeur = c;
        }
    }

    if (!meilleurChauffeur) {
        resultat.raisonEchec = "Aucun chauffeur disponible ne possede le permis requis (" +
                                toString(meilleurVehicule->permisRequis()) + ")";
        return resultat;
    }

    resultat.reussie = true;
    resultat.vehicule = meilleurVehicule;
    resultat.chauffeur = meilleurChauffeur;
    resultat.score = meilleurScore;
    return resultat;
}

} // namespace sl