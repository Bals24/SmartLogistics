#include "strategies/StrategiesConcretes.hpp"
#include <limits>

namespace sl {

namespace {
constexpr double INFINI = std::numeric_limits<double>::infinity();
}

double StrategiePlusCourtChemin::poids(const Route& route) const {
    if (route.getEtat() == EtatRoute::BLOQUEE) return INFINI;
    return route.getDistanceKm();
}
std::string StrategiePlusCourtChemin::nom() const { return "PLUS_COURT_CHEMIN"; }

double StrategiePlusRapide::poids(const Route& route) const {
    if (route.getEtat() == EtatRoute::BLOQUEE) return INFINI;
    double facteurRalentissement = (route.getEtat() == EtatRoute::RALENTIE) ? 1.5 : 1.0;
    return route.getTempsHeures() * facteurRalentissement;
}
std::string StrategiePlusRapide::nom() const { return "PLUS_RAPIDE"; }

double StrategieMoinsCher::poids(const Route& route) const {
    if (route.getEtat() == EtatRoute::BLOQUEE) return INFINI;
    return route.getCoutAr();
}
std::string StrategieMoinsCher::nom() const { return "MOINS_CHER"; }

} // namespace sl
