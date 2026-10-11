#include "entities/Incident.hpp"
#include <iostream>

namespace sl {

Incident::Incident(std::string id,
                    std::string description,
                    std::string localisation,
                    std::string livraisonId,
                    GraviteIncident gravite)
    : id_(std::move(id)),
      description_(std::move(description)),
      localisation_(std::move(localisation)),
      livraisonId_(std::move(livraisonId)),
      gravite_(gravite),
      dateCreation_(std::time(nullptr)) {}

const std::string& Incident::getId() const { return id_; }
const std::string& Incident::getDescription() const { return description_; }
const std::string& Incident::getLocalisation() const { return localisation_; }
const std::string& Incident::getLivraisonId() const { return livraisonId_; }
GraviteIncident Incident::getGravite() const { return gravite_; }
std::time_t Incident::getDateCreation() const { return dateCreation_; }

bool Incident::bloqueLivraison() const { return false; }

void Incident::afficherEntete() const {
    std::cout << "/!\\ INCIDENT #" << id_ << " (" << getType() << ")\n";
    std::cout << "Description  : " << description_ << "\n";
    std::cout << "Localisation : " << localisation_ << "\n";
    std::cout << "Gravite      : " << toString(gravite_) << "\n";
    std::cout << "Livraison    : " << livraisonId_ << "\n";
}

void Incident::afficherInformations() const {
    afficherEntete();
}

} // namespace sl
