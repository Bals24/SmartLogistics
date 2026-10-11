#ifndef SMARTLOGISTICS_VEHICULEFACTORY_HPP
#define SMARTLOGISTICS_VEHICULEFACTORY_HPP

#include <memory>
#include <string>
#include "entities/Vehicule.hpp"

namespace sl {

// Paramètres optionnels selon le type de véhicule (ex: température de
// consigne pour un véhicule frigorifique uniquement).
struct ParametresVehicule {
    std::string immatriculation;
    std::string marque;
    std::string modele;
    double capaciteKg = 0.0;
    double kilometrage = 0.0;
    double niveauCarburantPct = 100.0;
    double temperatureConsigneC = 4.0; // utilisé seulement pour FRIGORIFIQUE
};

// Factory Pattern : centralise la création des véhicules concrets à partir
// d'une chaîne de type, pour ne pas disperser les `new Camion(...)` etc.
// dans tout le code (utile notamment lors du chargement JSON, Niveau 4).
class VehiculeFactory {
public:
    // Types acceptés : "CAMION", "FOURGON", "MOTO", "FRIGORIFIQUE".
    // Lève TypeInconnuException si le type n'est pas reconnu.
    static std::unique_ptr<Vehicule> creer(const std::string& type,
                                            const ParametresVehicule& parametres);
};

} // namespace sl

#endif // SMARTLOGISTICS_VEHICULEFACTORY_HPP
