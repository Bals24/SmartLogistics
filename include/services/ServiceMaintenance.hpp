#ifndef SMARTLOGISTICS_SERVICEMAINTENANCE_HPP
#define SMARTLOGISTICS_SERVICEMAINTENANCE_HPP

#include <vector>
#include <memory>
#include <mutex>
#include "entities/Maintenance.hpp"
#include "services/GestionnaireFlotte.hpp"

namespace sl {

// Tient l'historique des operations de maintenance et sait detecter, a
// partir du kilometrage actuel de chaque vehicule de la flotte, lesquels
// ont depasse le seuil fixe par leur derniere Maintenance enregistree.
// Utilise a la fois par le menu console (verification manuelle) et par
// SurveillanceFlotte (verification periodique en arriere-plan, Niveau 6).
class ServiceMaintenance {
public:
    void enregistrerMaintenance(std::unique_ptr<Maintenance> maintenance);

    // Renvoie les vehicules dont la maintenance est requise, et bascule
    // leur statut a EN_MAINTENANCE au passage.
    std::vector<Vehicule*> detecterEtMarquer(GestionnaireFlotte& flotte);

    const Maintenance* derniereMaintenancePour(const std::string& vehiculeId) const;
    std::size_t nombreEnregistrements() const;

private:
    mutable std::mutex mutex_;
    std::vector<std::unique_ptr<Maintenance>> historique_;
};

} // namespace sl

#endif // SMARTLOGISTICS_SERVICEMAINTENANCE_HPP
