#include "entities/Maintenance.hpp"
#include <iostream>

namespace sl {

Maintenance::Maintenance(std::string id,
                          std::string vehiculeId,
                          TypeMaintenance type,
                          double kilometrageEffectue,
                          double intervalleKm,
                          std::string description)
    : id_(std::move(id)), vehiculeId_(std::move(vehiculeId)), type_(type),
      kilometrageEffectue_(kilometrageEffectue), intervalleKm_(intervalleKm),
      description_(std::move(description)) {}

const std::string& Maintenance::getId() const { return id_; }
const std::string& Maintenance::getVehiculeId() const { return vehiculeId_; }
TypeMaintenance Maintenance::getType() const { return type_; }
double Maintenance::getKilometrageEffectue() const { return kilometrageEffectue_; }
double Maintenance::getKilometrageProchain() const { return kilometrageEffectue_ + intervalleKm_; }
const std::string& Maintenance::getDescription() const { return description_; }

bool Maintenance::maintenanceRequise(double kilometrageActuel) const {
    return kilometrageActuel >= getKilometrageProchain();
}

void Maintenance::afficherInformations() const {
    std::cout << "------------------------------\n";
    std::cout << "MAINTENANCE #" << id_ << " (vehicule " << vehiculeId_ << ")\n";
    std::cout << "Type              : " << toString(type_) << "\n";
    std::cout << "Derniere a        : " << kilometrageEffectue_ << " km\n";
    std::cout << "Prochaine a       : " << getKilometrageProchain() << " km\n";
    std::cout << "Description       : " << description_ << "\n";
    std::cout << "------------------------------\n";
}

} // namespace sl
