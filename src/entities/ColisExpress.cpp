#include "entities/ColisExpress.hpp"
#include <iostream>

namespace sl {

ColisExpress::ColisExpress(std::string id,
                            std::string description,
                            double poidsKg,
                            double volumeM3,
                            std::string origine,
                            std::string destination,
                            int delaiMaxHeures)
    : Colis(std::move(id), std::move(description), poidsKg, volumeM3,
            std::move(origine), std::move(destination), Priorite::EXPRESS),
      delaiMaxHeures_(delaiMaxHeures) {}

std::string ColisExpress::getType() const { return "EXPRESS"; }

int ColisExpress::getDelaiMaxHeures() const { return delaiMaxHeures_; }

void ColisExpress::afficherInformations() const {
    afficherEntete();
    std::cout << "Delai max   : " << delaiMaxHeures_ << " heures\n";
    std::cout << "==============================\n";
}

} // namespace sl
