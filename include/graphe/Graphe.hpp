#ifndef SMARTLOGISTICS_GRAPHE_HPP
#define SMARTLOGISTICS_GRAPHE_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include "graphe/Route.hpp"

namespace sl {

// Graphe non orienté représentant le réseau routier entre les villes
// desservies par Smart Logistics. Implémenté en liste d'adjacence.
class Graphe {
public:
    // Ajoute une route entre deux villes. Par défaut bidirectionnelle
    // (on peut aller de A vers B et de B vers A avec les mêmes caractéristiques).
    void ajouterRoute(const Route& route, bool bidirectionnelle = true);

    const std::vector<Route>& getRoutesDepuis(const std::string& ville) const;
    std::vector<std::string> getVilles() const;
    bool contientVille(const std::string& ville) const;

private:
    std::unordered_map<std::string, std::vector<Route>> adjacence_;
    static const std::vector<Route> routesVides_;
};

} // namespace sl

#endif // SMARTLOGISTICS_GRAPHE_HPP
