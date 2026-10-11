#include "entities/VehiculeFrigorifique.hpp"
#include <iostream>

namespace sl {

VehiculeFrigorifique::VehiculeFrigorifique(std::string immatriculation,
                                            std::string marque,
                                            std::string modele,
                                            double capaciteKg,
                                            double temperatureConsigneC,
                                            double kilometrage,
                                            double niveauCarburantPct)
    : Vehicule(std::move(immatriculation), std::move(marque), std::move(modele),
               capaciteKg, kilometrage, niveauCarburantPct),
      temperatureConsigneC_(temperatureConsigneC) {}

std::string VehiculeFrigorifique::getType() const { return "VEHICULE_FRIGORIFIQUE"; }
TypePermis VehiculeFrigorifique::permisRequis() const { return TypePermis::C1; }
bool VehiculeFrigorifique::estFrigorifique() const { return true; }

double VehiculeFrigorifique::getTemperatureConsigneC() const {
    return temperatureConsigneC_;
}

void VehiculeFrigorifique::definirTemperatureConsigne(double tempC) {
    temperatureConsigneC_ = tempC;
}

void VehiculeFrigorifique::afficherInformations() const {
    afficherEntete();
    std::cout << "Temp. consigne: " << temperatureConsigneC_ << " C\n";
    std::cout << "------------------------------\n";
}

} // namespace sl
