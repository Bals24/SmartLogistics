#include "entities/Livraison.hpp"
#include <iostream>
#include <algorithm>

namespace sl {

Livraison::Livraison(std::string id,
                      Commande* commande,
                      Vehicule* vehicule,
                      Chauffeur* chauffeur,
                      Itineraire itineraire)
    : id_(std::move(id)), commande_(commande), vehicule_(vehicule),
      chauffeur_(chauffeur), itineraire_(std::move(itineraire)),
      statut_(StatutLivraison::PLANIFIEE) {}

const std::string& Livraison::getId() const { return id_; }
Commande* Livraison::getCommande() const { return commande_; }
Vehicule* Livraison::getVehicule() const { return vehicule_; }
Chauffeur* Livraison::getChauffeur() const { return chauffeur_; }
const Itineraire& Livraison::getItineraire() const { return itineraire_; }
StatutLivraison Livraison::getStatut() const { return statut_; }

void Livraison::setStatut(StatutLivraison statut) { statut_ = statut; }

void Livraison::ajouterIncident(std::unique_ptr<Incident> incident) {
    incidents_.push_back(std::move(incident));
}

const std::vector<std::unique_ptr<Incident>>& Livraison::getIncidents() const {
    return incidents_;
}

bool Livraison::estBloquee() const {
    return std::any_of(incidents_.begin(), incidents_.end(),
        [](const std::unique_ptr<Incident>& i) { return i->bloqueLivraison(); });
}

void Livraison::afficherInformations() const {
    std::cout << "==============================\n";
    std::cout << "LIVRAISON #" << id_ << "\n";
    std::cout << "==============================\n";
    if (commande_) std::cout << "Commande  : " << commande_->getId() << "\n";
    if (vehicule_) std::cout << "Vehicule  : " << vehicule_->getImmatriculation()
                              << " (" << vehicule_->getType() << ")\n";
    if (chauffeur_) std::cout << "Chauffeur : " << chauffeur_->getNom() << "\n";
    std::cout << "Statut    : " << toString(statut_) << "\n";
    itineraire_.afficherInformations();
    if (!incidents_.empty()) {
        std::cout << "Incidents (" << incidents_.size() << ") :\n";
        for (const auto& incident : incidents_) {
            incident->afficherInformations();
        }
    }
    std::cout << "==============================\n";
}

} // namespace sl
