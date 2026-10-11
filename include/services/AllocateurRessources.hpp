#ifndef SMARTLOGISTICS_ALLOCATEURRESSOURCES_HPP
#define SMARTLOGISTICS_ALLOCATEURRESSOURCES_HPP

#include <vector>
#include <string>
#include "entities/Commande.hpp"
#include "entities/Vehicule.hpp"
#include "entities/Chauffeur.hpp"

namespace sl {

// Resultat d'une tentative d'affectation automatique de ressources.
struct ResultatAffectation {
    bool reussie = false;
    Vehicule* vehicule = nullptr;
    Chauffeur* chauffeur = nullptr;
    double score = 0.0;
    std::string raisonEchec;
};

// Moteur de decision : choisit automatiquement le meilleur couple
// (vehicule, chauffeur) disponible pour une commande donnee, en excluant
// les candidats incompatibles : capacite insuffisante, pas de vehicule
// frigorifique alors qu'il en faut un, ou permis du chauffeur incompatible
// avec le vehicule retenu.
//
// Score du vehicule = compromis entre "peu de gaspillage de capacite"
// (on evite d'envoyer un camion de 5T pour un colis de 10kg) et un bon
// niveau de carburant restant. Le chauffeur retenu est, parmi ceux ayant
// le bon permis, celui qui a le plus d'experience.
class AllocateurRessources {
public:
    static ResultatAffectation affecter(const Commande& commande,
                                         const std::vector<Vehicule*>& vehiculesDisponibles,
                                         const std::vector<Chauffeur*>& chauffeursDisponibles);

private:
    static double calculerScore(const Vehicule& vehicule, double poidsCommandeKg);
};

} // namespace sl

#endif // SMARTLOGISTICS_ALLOCATEURRESSOURCES_HPP
