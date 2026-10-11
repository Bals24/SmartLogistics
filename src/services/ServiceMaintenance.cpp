#include "services/ServiceMaintenance.hpp"

namespace sl {

void ServiceMaintenance::enregistrerMaintenance(std::unique_ptr<Maintenance> maintenance) {
    std::lock_guard<std::mutex> verrou(mutex_);
    historique_.push_back(std::move(maintenance));
}

const Maintenance* ServiceMaintenance::derniereMaintenancePour(const std::string& vehiculeId) const {
    const Maintenance* derniere = nullptr;
    for (const auto& m : historique_) {
        if (m->getVehiculeId() == vehiculeId) {
            // On garde la derniere rencontree (les enregistrements sont
            // ajoutes dans l'ordre chronologique).
            derniere = m.get();
        }
    }
    return derniere;
}

std::vector<Vehicule*> ServiceMaintenance::detecterEtMarquer(GestionnaireFlotte& flotte) {
    std::vector<Vehicule*> aMaintenir;

    std::lock_guard<std::mutex> verrou(mutex_);
    for (Vehicule* v : flotte.tousLesVehicules()) {
        const Maintenance* derniere = nullptr;
        for (const auto& m : historique_) {
            if (m->getVehiculeId() == v->getImmatriculation()) derniere = m.get();
        }
        if (derniere && derniere->maintenanceRequise(v->getKilometrage())) {
            v->setStatut(StatutVehicule::EN_MAINTENANCE);
            aMaintenir.push_back(v);
        }
    }
    return aMaintenir;
}

std::size_t ServiceMaintenance::nombreEnregistrements() const {
    std::lock_guard<std::mutex> verrou(mutex_);
    return historique_.size();
}

} // namespace sl
