#include "graphe/Route.hpp"

namespace sl {

Route::Route(std::string villeA, std::string villeB,
             double distanceKm, double tempsHeures, double coutAr, EtatRoute etat)
    : villeA_(std::move(villeA)), villeB_(std::move(villeB)),
      distanceKm_(distanceKm), tempsHeures_(tempsHeures), coutAr_(coutAr), etat_(etat) {}

const std::string& Route::getVilleA() const { return villeA_; }
const std::string& Route::getVilleB() const { return villeB_; }
double Route::getDistanceKm() const { return distanceKm_; }
double Route::getTempsHeures() const { return tempsHeures_; }
double Route::getCoutAr() const { return coutAr_; }
EtatRoute Route::getEtat() const { return etat_; }

void Route::setEtat(EtatRoute etat) { etat_ = etat; }

std::string Route::autreVille(const std::string& ville) const {
    return (ville == villeA_) ? villeB_ : villeA_;
}

} // namespace sl
