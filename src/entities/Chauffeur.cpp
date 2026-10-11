#include "entities/Chauffeur.hpp"
#include <iostream>

namespace sl {

Chauffeur::Chauffeur(std::string id,
                      std::string nom,
                      TypePermis typePermis,
                      int experienceAnnees)
    : id_(std::move(id)),
      nom_(std::move(nom)),
      typePermis_(typePermis),
      experienceAnnees_(experienceAnnees),
      statut_(StatutChauffeur::DISPONIBLE) {}

const std::string& Chauffeur::getId() const { return id_; }
const std::string& Chauffeur::getNom() const { return nom_; }
TypePermis Chauffeur::getTypePermis() const { return typePermis_; }
int Chauffeur::getExperienceAnnees() const { return experienceAnnees_; }
StatutChauffeur Chauffeur::getStatut() const { return statut_; }

void Chauffeur::setStatut(StatutChauffeur statut) { statut_ = statut; }

bool Chauffeur::peutConduire(TypePermis permisRequisPourVehicule) const {
    if (typePermis_ == permisRequisPourVehicule) return true;
    // Le permis poids lourd (C) couvre aussi les véhicules C1.
    if (typePermis_ == TypePermis::C && permisRequisPourVehicule == TypePermis::C1) {
        return true;
    }
    return false;
}

void Chauffeur::afficherInformations() const {
    std::cout << "------------------------------\n";
    std::cout << "CHAUFFEUR #" << id_ << "\n";
    std::cout << "Nom        : " << nom_ << "\n";
    std::cout << "Permis     : " << toString(typePermis_) << "\n";
    std::cout << "Experience : " << experienceAnnees_ << " ans\n";
    std::cout << "Statut     : " << toString(statut_) << "\n";
    std::cout << "------------------------------\n";
}

} // namespace sl
