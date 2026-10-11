#include "entities/ColisRefrigere.hpp"
#include <iostream>

namespace sl {

ColisRefrigere::ColisRefrigere(std::string id,
                                std::string description,
                                double poidsKg,
                                double volumeM3,
                                std::string origine,
                                std::string destination,
                                double temperatureMinC,
                                double temperatureMaxC,
                                Priorite priorite)
    : Colis(std::move(id), std::move(description), poidsKg, volumeM3,
            std::move(origine), std::move(destination), priorite),
      temperatureMinC_(temperatureMinC),
      temperatureMaxC_(temperatureMaxC) {}

std::string ColisRefrigere::getType() const { return "REFRIGERE"; }

bool ColisRefrigere::necessiteVehiculeSpecial() const { return true; }

double ColisRefrigere::getTemperatureMinC() const { return temperatureMinC_; }
double ColisRefrigere::getTemperatureMaxC() const { return temperatureMaxC_; }

void ColisRefrigere::afficherInformations() const {
    afficherEntete();
    std::cout << "Temperature : [" << temperatureMinC_ << "C , "
               << temperatureMaxC_ << "C]\n";
    std::cout << "Vehicule    : Frigorifique requis\n";
    std::cout << "==============================\n";
}

} // namespace sl
