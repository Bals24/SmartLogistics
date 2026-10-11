#ifndef SMARTLOGISTICS_ROUTE_HPP
#define SMARTLOGISTICS_ROUTE_HPP

#include <string>
#include "entities/Enums.hpp"

namespace sl {

// Une arête du graphe routier entre deux villes.
class Route {
public:
    Route(std::string villeA, std::string villeB,
          double distanceKm, double tempsHeures, double coutAr,
          EtatRoute etat = EtatRoute::LIBRE);

    const std::string& getVilleA() const;
    const std::string& getVilleB() const;
    double getDistanceKm() const;
    double getTempsHeures() const;
    double getCoutAr() const;
    EtatRoute getEtat() const;

    void setEtat(EtatRoute etat);

    // Renvoie la ville "de l'autre côté" de la route par rapport à celle donnée.
    std::string autreVille(const std::string& ville) const;

private:
    std::string villeA_;
    std::string villeB_;
    double distanceKm_;
    double tempsHeures_;
    double coutAr_;
    EtatRoute etat_;
};

} // namespace sl

#endif // SMARTLOGISTICS_ROUTE_HPP
