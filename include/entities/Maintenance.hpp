#ifndef SMARTLOGISTICS_MAINTENANCE_HPP
#define SMARTLOGISTICS_MAINTENANCE_HPP

#include <string>
#include "entities/Enums.hpp"

namespace sl {

class Maintenance {
public:
    Maintenance(std::string id,
                std::string vehiculeId,
                TypeMaintenance type,
                double kilometrageEffectue,
                double intervalleKm,
                std::string description);

    const std::string& getId() const;
    const std::string& getVehiculeId() const;
    TypeMaintenance getType() const;
    double getKilometrageEffectue() const;
    double getKilometrageProchain() const;
    const std::string& getDescription() const;

    // Renvoie vrai si le véhicule (à kilometrageActuel) a dépassé
    // le seuil de la prochaine maintenance.
    bool maintenanceRequise(double kilometrageActuel) const;

    void afficherInformations() const;

private:
    std::string id_;
    std::string vehiculeId_;
    TypeMaintenance type_;
    double kilometrageEffectue_;
    double intervalleKm_;
    std::string description_;
};

} // namespace sl

#endif // SMARTLOGISTICS_MAINTENANCE_HPP
