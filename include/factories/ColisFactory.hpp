#ifndef SMARTLOGISTICS_COLISFACTORY_HPP
#define SMARTLOGISTICS_COLISFACTORY_HPP

#include <memory>
#include <string>
#include <optional>
#include "entities/Colis.hpp"

namespace sl {

// Paramètres communs + champs optionnels spécifiques à chaque sous-type,
// pour garder une seule signature de création malgré des constructeurs
// différents (ColisFragile a une consigne, ColisExpress un délai, etc.).
struct ParametresColis {
    std::string id;
    std::string description;
    double poidsKg = 0.0;
    double volumeM3 = 0.0;
    std::string origine;
    std::string destination;
    Priorite priorite = Priorite::STANDARD;

    std::optional<std::string> consigneManutention;  // pour FRAGILE
    std::optional<int> delaiMaxHeures;                // pour EXPRESS
    std::optional<double> temperatureMinC;            // pour REFRIGERE
    std::optional<double> temperatureMaxC;            // pour REFRIGERE
};

// Factory Pattern : centralise la création des colis concrets.
// Types acceptés : "STANDARD", "FRAGILE", "EXPRESS", "REFRIGERE".
// Lève TypeInconnuException si le type est inconnu, ou SmartLogisticsException
// si un champ requis pour le type demandé est manquant.
class ColisFactory {
public:
    static std::unique_ptr<Colis> creer(const std::string& type,
                                         const ParametresColis& parametres);
};

} // namespace sl

#endif // SMARTLOGISTICS_COLISFACTORY_HPP
