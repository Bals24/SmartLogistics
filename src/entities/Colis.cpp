#include "entities/Colis.hpp"
#include <iostream>
#include <iomanip>

namespace sl {

Colis::Colis(std::string id,
             std::string description,
             double poidsKg,
             double volumeM3,
             std::string origine,
             std::string destination,
             Priorite priorite)
    : id_(std::move(id)),
      description_(std::move(description)),
      poidsKg_(poidsKg),
      volumeM3_(volumeM3),
      origine_(std::move(origine)),
      destination_(std::move(destination)),
      priorite_(priorite),
      statut_(StatutColis::EN_ATTENTE) {}

const std::string& Colis::getId() const { return id_; }
const std::string& Colis::getDescription() const { return description_; }
double Colis::getPoidsKg() const { return poidsKg_; }
double Colis::getVolumeM3() const { return volumeM3_; }
const std::string& Colis::getOrigine() const { return origine_; }
const std::string& Colis::getDestination() const { return destination_; }
Priorite Colis::getPriorite() const { return priorite_; }
StatutColis Colis::getStatut() const { return statut_; }

void Colis::setStatut(StatutColis nouveauStatut) { statut_ = nouveauStatut; }

bool Colis::necessiteVehiculeSpecial() const { return false; }

void Colis::afficherEntete() const {
    std::cout << "==============================\n";
    std::cout << "COLIS #" << id_ << " (" << getType() << ")\n";
    std::cout << "==============================\n";
    std::cout << "Description : " << description_ << "\n";
    std::cout << "Poids       : " << poidsKg_ << " kg\n";
    std::cout << "Volume      : " << std::fixed << std::setprecision(3)
               << volumeM3_ << " m3\n";
    std::cout << "Origine     : " << origine_ << "\n";
    std::cout << "Destination : " << destination_ << "\n";
    std::cout << "Priorite    : " << toString(priorite_) << "\n";
    std::cout << "Statut      : " << toString(statut_) << "\n";
}

void Colis::afficherInformations() const {
    afficherEntete();
    std::cout << "==============================\n";
}

} // namespace sl
