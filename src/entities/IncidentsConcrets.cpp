#include "entities/IncidentsConcrets.hpp"
#include <iostream>

namespace sl {

// ---------- Accident ----------
Accident::Accident(std::string id, std::string description, std::string localisation,
                    std::string livraisonId, GraviteIncident gravite)
    : Incident(std::move(id), std::move(description), std::move(localisation),
               std::move(livraisonId), gravite) {}

std::string Accident::getType() const { return "ACCIDENT"; }
bool Accident::bloqueLivraison() const { return true; }

// ---------- Panne ----------
Panne::Panne(std::string id, std::string description, std::string localisation,
             std::string livraisonId, std::string typePanne, GraviteIncident gravite)
    : Incident(std::move(id), std::move(description), std::move(localisation),
               std::move(livraisonId), gravite),
      typePanne_(std::move(typePanne)) {}

std::string Panne::getType() const { return "PANNE"; }
bool Panne::bloqueLivraison() const { return true; }
const std::string& Panne::getTypePanne() const { return typePanne_; }

void Panne::afficherInformations() const {
    afficherEntete();
    std::cout << "Type panne   : " << typePanne_ << "\n";
}

// ---------- RouteBloquee ----------
RouteBloquee::RouteBloquee(std::string id, std::string description, std::string localisation,
                           std::string livraisonId, GraviteIncident gravite)
    : Incident(std::move(id), std::move(description), std::move(localisation),
               std::move(livraisonId), gravite) {}

std::string RouteBloquee::getType() const { return "ROUTE_BLOQUEE"; }
bool RouteBloquee::bloqueLivraison() const { return true; }

// ---------- Retard ----------
Retard::Retard(std::string id, std::string description, std::string localisation,
               std::string livraisonId, int retardEstimeMinutes, GraviteIncident gravite)
    : Incident(std::move(id), std::move(description), std::move(localisation),
               std::move(livraisonId), gravite),
      retardEstimeMinutes_(retardEstimeMinutes) {}

std::string Retard::getType() const { return "RETARD"; }
bool Retard::bloqueLivraison() const { return false; }
int Retard::getRetardEstimeMinutes() const { return retardEstimeMinutes_; }

void Retard::afficherInformations() const {
    afficherEntete();
    std::cout << "Retard estime: " << retardEstimeMinutes_ << " minutes\n";
}

// ---------- ColisEndommage ----------
ColisEndommage::ColisEndommage(std::string id, std::string description, std::string localisation,
                               std::string livraisonId, std::string colisId, GraviteIncident gravite)
    : Incident(std::move(id), std::move(description), std::move(localisation),
               std::move(livraisonId), gravite),
      colisId_(std::move(colisId)) {}

std::string ColisEndommage::getType() const { return "COLIS_ENDOMMAGE"; }
bool ColisEndommage::bloqueLivraison() const { return false; }
const std::string& ColisEndommage::getColisId() const { return colisId_; }

void ColisEndommage::afficherInformations() const {
    afficherEntete();
    std::cout << "Colis concerne: " << colisId_ << "\n";
}

} // namespace sl
