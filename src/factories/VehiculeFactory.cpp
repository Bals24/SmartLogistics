#include "factories/VehiculeFactory.hpp"
#include "entities/Camion.hpp"
#include "entities/Fourgon.hpp"
#include "entities/Moto.hpp"
#include "entities/VehiculeFrigorifique.hpp"
#include "utils/Exceptions.hpp"
#include <algorithm>

namespace sl {

namespace {
std::string versMajuscules(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    return s;
}
}

std::unique_ptr<Vehicule> VehiculeFactory::creer(const std::string& type,
                                                  const ParametresVehicule& p) {
    std::string typeNormalise = versMajuscules(type);

    if (typeNormalise == "CAMION") {
        return std::make_unique<Camion>(p.immatriculation, p.marque, p.modele,
                                         p.capaciteKg, p.kilometrage, p.niveauCarburantPct);
    }
    if (typeNormalise == "FOURGON") {
        return std::make_unique<Fourgon>(p.immatriculation, p.marque, p.modele,
                                          p.capaciteKg, p.kilometrage, p.niveauCarburantPct);
    }
    if (typeNormalise == "MOTO") {
        return std::make_unique<Moto>(p.immatriculation, p.marque, p.modele,
                                       p.capaciteKg, p.kilometrage, p.niveauCarburantPct);
    }
    if (typeNormalise == "FRIGORIFIQUE" || typeNormalise == "VEHICULE_FRIGORIFIQUE") {
        return std::make_unique<VehiculeFrigorifique>(p.immatriculation, p.marque, p.modele,
                                                        p.capaciteKg, p.temperatureConsigneC,
                                                        p.kilometrage, p.niveauCarburantPct);
    }
    throw TypeInconnuException(type);
}

} // namespace sl
