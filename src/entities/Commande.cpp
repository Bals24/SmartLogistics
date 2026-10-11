#include "entities/Commande.hpp"
#include "etats/EtatsCommandeConcrets.hpp"
#include <iostream>
#include <numeric>
#include <algorithm>

namespace sl {

Commande::Commande(std::string id,
                    const Client* client,
                    std::string destination,
                    Priorite priorite)
    : id_(std::move(id)),
      client_(client),
      destination_(std::move(destination)),
      priorite_(priorite),
      etat_(std::make_unique<EtatCree>()) {}

const std::string& Commande::getId() const { return id_; }
const Client* Commande::getClient() const { return client_; }
const std::string& Commande::getDestination() const { return destination_; }
Priorite Commande::getPriorite() const { return priorite_; }

StatutCommande Commande::getStatut() const { return etat_->statut(); }
std::string Commande::getNomEtat() const { return etat_->nom(); }

void Commande::definirEtat(std::unique_ptr<EtatCommande> nouvelEtat) {
    etat_ = std::move(nouvelEtat);
}

void Commande::avancer() {
    StatutCommande ancien = etat_->statut();
    etat_->avancer(*this);           // peut remplacer etat_ (voir definirEtat)
    StatutCommande nouveau = etat_->statut();
    if (ancien != nouveau) notifierObservateurs(ancien, nouveau);
}

void Commande::annulerCommande() {
    StatutCommande ancien = etat_->statut();
    etat_->annuler(*this);
    StatutCommande nouveau = etat_->statut();
    if (ancien != nouveau) notifierObservateurs(ancien, nouveau);
}

void Commande::declarerIncident() {
    StatutCommande ancien = etat_->statut();
    etat_->declarerIncident(*this);
    StatutCommande nouveau = etat_->statut();
    if (ancien != nouveau) notifierObservateurs(ancien, nouveau);
}

void Commande::abonner(IObservateurCommande* observateur) {
    if (observateur) observateurs_.push_back(observateur);
}

void Commande::notifierObservateurs(StatutCommande ancien, StatutCommande nouveau) const {
    for (auto* observateur : observateurs_) {
        observateur->notifier(*this, ancien, nouveau);
    }
}

void Commande::ajouterColis(std::unique_ptr<Colis> colis) {
    colis_.push_back(std::move(colis));
}

const std::vector<std::unique_ptr<Colis>>& Commande::getColis() const { return colis_; }

double Commande::poidsTotalKg() const {
    return std::accumulate(colis_.begin(), colis_.end(), 0.0,
        [](double somme, const std::unique_ptr<Colis>& c) {
            return somme + c->getPoidsKg();
        });
}

double Commande::volumeTotalM3() const {
    return std::accumulate(colis_.begin(), colis_.end(), 0.0,
        [](double somme, const std::unique_ptr<Colis>& c) {
            return somme + c->getVolumeM3();
        });
}

bool Commande::necessiteVehiculeSpecial() const {
    return std::any_of(colis_.begin(), colis_.end(),
        [](const std::unique_ptr<Colis>& c) {
            return c->necessiteVehiculeSpecial();
        });
}

void Commande::afficherInformations() const {
    std::cout << "==============================\n";
    std::cout << "COMMANDE #" << id_ << "\n";
    std::cout << "==============================\n";
    if (client_) {
        std::cout << "Client      : " << client_->getNom() << "\n";
    }
    std::cout << "Destination : " << destination_ << "\n";
    std::cout << "Priorite    : " << toString(priorite_) << "\n";
    std::cout << "Statut      : " << getNomEtat() << "\n";
    std::cout << "Colis (" << colis_.size() << ") :\n";
    for (const auto& c : colis_) {
        std::cout << "  - " << c->getId() << " (" << c->getType() << ")\n";
    }
    std::cout << "Poids total : " << poidsTotalKg() << " kg\n";
    std::cout << "==============================\n";
}

} // namespace sl
