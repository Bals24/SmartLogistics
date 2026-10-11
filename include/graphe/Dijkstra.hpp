#ifndef SMARTLOGISTICS_DIJKSTRA_HPP
#define SMARTLOGISTICS_DIJKSTRA_HPP

#include <string>
#include "graphe/Graphe.hpp"
#include "graphe/Itineraire.hpp"
#include "strategies/StrategieItineraire.hpp"

namespace sl {

// Calcule le meilleur chemin entre deux villes du graphe, où "meilleur"
// est défini par la strategie fournie (Strategy Pattern). Les totaux
// distance/temps/coût de l'Itineraire renvoyé sont toujours les vraies
// valeurs cumulées le long du chemin retenu, quelle que soit la stratégie.
Itineraire calculerPlusCourtChemin(const Graphe& graphe,
                                    const std::string& depart,
                                    const std::string& arrivee,
                                    const StrategieItineraire& strategie);

} // namespace sl

#endif // SMARTLOGISTICS_DIJKSTRA_HPP
