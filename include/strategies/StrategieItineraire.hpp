#ifndef SMARTLOGISTICS_STRATEGIEITINERAIRE_HPP
#define SMARTLOGISTICS_STRATEGIEITINERAIRE_HPP

#include <string>
#include "graphe/Route.hpp"

namespace sl {

// Strategy Pattern : chaque stratégie définit comment "peser" une route.
// Dijkstra utilise ce poids pour décider du meilleur chemin, mais les totaux
// réels (distance/temps/coût) sont toujours accumulés séparément.
class StrategieItineraire {
public:
    virtual ~StrategieItineraire() = default;

    // Une route à l'état BLOQUEE est toujours considérée impraticable,
    // quelle que soit la stratégie (poids infini).
    virtual double poids(const Route& route) const = 0;
    virtual std::string nom() const = 0;
};

} // namespace sl

#endif // SMARTLOGISTICS_STRATEGIEITINERAIRE_HPP
