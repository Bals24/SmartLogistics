#include "entities/Vehicule.hpp"
#include <iostream>
#include <algorithm>

namespace sl {

Vehicule::Vehicule(std::string immatriculation,
                    std::string marque,
                    std::string modele,
                    double capaciteKg,
                    double kilometrage,
                    double niveauCarburantPct)
    : immatriculation_(std::move(immatriculation)),
      marque_(std::move(marque)),
      modele_(std::move(modele)),
      capaciteKg_(capaciteKg),
      chargeActuelleKg_(0.0),
      kilometrage_(kilometrage),
      niveauCarburantPct_(niveauCarburantPct),
      statut_(StatutVehicule::DISPONIBLE) {}

const std::string& Vehicule::getImmatriculation() const { return immatriculation_; }
const std::string& Vehicule::getMarque() const { return marque_; }
const std::string& Vehicule::getModele() const { return modele_; }
double Vehicule::getCapaciteKg() const { return capaciteKg_; }
double Vehicule::getChargeActuelleKg() const { return chargeActuelleKg_; }
double Vehicule::getKilometrage() const { return kilometrage_; }
double Vehicule::getNiveauCarburantPct() const { return niveauCarburantPct_; }
StatutVehicule Vehicule::getStatut() const { return statut_; }

void Vehicule::setStatut(StatutVehicule statut) { statut_ = statut; }

void Vehicule::ajouterKilometrage(double km) {
    if (km > 0) kilometrage_ += km;
}

void Vehicule::definirNiveauCarburant(double pct) {
    niveauCarburantPct_ = std::clamp(pct, 0.0, 100.0);
}

double Vehicule::getCapaciteRestanteKg() const {
    return capaciteKg_ - chargeActuelleKg_;
}

bool Vehicule::chargerPoids(double poidsKg) {
    if (poidsKg <= 0) return false;
    if (poidsKg > getCapaciteRestanteKg()) return false;
    chargeActuelleKg_ += poidsKg;
    return true;
}

void Vehicule::viderChargement() { chargeActuelleKg_ = 0.0; }

bool Vehicule::estFrigorifique() const { return false; }

void Vehicule::afficherEntete() const {
    std::cout << "------------------------------\n";
    std::cout << getType() << " " << immatriculation_
               << " (" << marque_ << " " << modele_ << ")\n";
    std::cout << "Capacite      : " << capaciteKg_ << " kg\n";
    std::cout << "Charge        : " << chargeActuelleKg_ << " kg\n";
    std::cout << "Kilometrage   : " << kilometrage_ << " km\n";
    std::cout << "Carburant     : " << niveauCarburantPct_ << " %\n";
    std::cout << "Statut        : " << toString(statut_) << "\n";
}

void Vehicule::afficherInformations() const {
    afficherEntete();
    std::cout << "------------------------------\n";
}

} // namespace sl
