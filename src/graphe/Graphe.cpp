#include "graphe/Graphe.hpp"

namespace sl {

const std::vector<Route> Graphe::routesVides_ = {};

void Graphe::ajouterRoute(const Route& route, bool bidirectionnelle) {
    adjacence_[route.getVilleA()].push_back(route);
    if (bidirectionnelle) {
        // On crée l'arête inverse avec les mêmes caractéristiques.
        adjacence_[route.getVilleB()].push_back(
            Route(route.getVilleB(), route.getVilleA(),
                  route.getDistanceKm(), route.getTempsHeures(),
                  route.getCoutAr(), route.getEtat()));
    }
}

const std::vector<Route>& Graphe::getRoutesDepuis(const std::string& ville) const {
    auto it = adjacence_.find(ville);
    if (it == adjacence_.end()) return routesVides_;
    return it->second;
}

std::vector<std::string> Graphe::getVilles() const {
    std::vector<std::string> villes;
    villes.reserve(adjacence_.size());
    for (const auto& [ville, routes] : adjacence_) {
        villes.push_back(ville);
    }
    return villes;
}

bool Graphe::contientVille(const std::string& ville) const {
    return adjacence_.find(ville) != adjacence_.end();
}

} // namespace sl
