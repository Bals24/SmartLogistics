#ifndef SMARTLOGISTICS_GESTIONNAIREFLOTTE_HPP
#define SMARTLOGISTICS_GESTIONNAIREFLOTTE_HPP

#include <vector>
#include <memory>
#include <mutex>
#include <string>
#include "entities/Vehicule.hpp"

namespace sl {

// Centralise la gestion de la flotte : ajout, recherche, disponibilite.
// Toutes les operations sont protegees par un mutex interne, ce qui rend
// la classe utilisable en toute securite depuis plusieurs threads (voir
// SurveillanceFlotte, Niveau 6) en plus du thread principal (menu console).
class GestionnaireFlotte {
public:
    void ajouterVehicule(std::unique_ptr<Vehicule> vehicule);
    Vehicule* trouverParImmatriculation(const std::string& immatriculation);
    std::vector<Vehicule*> tousLesVehicules();
    std::vector<Vehicule*> vehiculesDisponibles();
    std::size_t taille() const;

private:
    mutable std::mutex mutex_;
    std::vector<std::unique_ptr<Vehicule>> flotte_;
};

} // namespace sl

#endif // SMARTLOGISTICS_GESTIONNAIREFLOTTE_HPP
