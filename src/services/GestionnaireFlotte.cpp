#include "services/GestionnaireFlotte.hpp"
#include <algorithm>

namespace sl {

void GestionnaireFlotte::ajouterVehicule(std::unique_ptr<Vehicule> vehicule) {
    std::lock_guard<std::mutex> verrou(mutex_);
    flotte_.push_back(std::move(vehicule));
}

Vehicule* GestionnaireFlotte::trouverParImmatriculation(const std::string& immatriculation) {
    std::lock_guard<std::mutex> verrou(mutex_);
    for (auto& v : flotte_) {
        if (v->getImmatriculation() == immatriculation) return v.get();
    }
    return nullptr;
}

std::vector<Vehicule*> GestionnaireFlotte::tousLesVehicules() {
    std::lock_guard<std::mutex> verrou(mutex_);
    std::vector<Vehicule*> resultat;
    resultat.reserve(flotte_.size());
    for (auto& v : flotte_) resultat.push_back(v.get());
    return resultat;
}

std::vector<Vehicule*> GestionnaireFlotte::vehiculesDisponibles() {
    std::lock_guard<std::mutex> verrou(mutex_);
    std::vector<Vehicule*> resultat;
    for (auto& v : flotte_) {
        if (v->getStatut() == StatutVehicule::DISPONIBLE) resultat.push_back(v.get());
    }
    return resultat;
}

std::size_t GestionnaireFlotte::taille() const {
    std::lock_guard<std::mutex> verrou(mutex_);
    return flotte_.size();
}

} // namespace sl
