#include "etats/EtatsCommandeConcrets.hpp"
#include "entities/Commande.hpp"
#include "utils/Exceptions.hpp"

namespace sl {

// ---------- EtatCree ----------
void EtatCree::avancer(Commande& contexte) {
    contexte.definirEtat(std::make_unique<EtatConfirmee>());
}
void EtatCree::annuler(Commande& contexte) {
    contexte.definirEtat(std::make_unique<EtatAnnulee>());
}
void EtatCree::declarerIncident(Commande& contexte) {
    contexte.definirEtat(std::make_unique<EtatIncident>());
}
StatutCommande EtatCree::statut() const { return StatutCommande::CREEE; }
std::string EtatCree::nom() const { return "CREEE"; }

// ---------- EtatConfirmee ----------
void EtatConfirmee::avancer(Commande& contexte) {
    contexte.definirEtat(std::make_unique<EtatPreparation>());
}
void EtatConfirmee::annuler(Commande& contexte) {
    contexte.definirEtat(std::make_unique<EtatAnnulee>());
}
void EtatConfirmee::declarerIncident(Commande& contexte) {
    contexte.definirEtat(std::make_unique<EtatIncident>());
}
StatutCommande EtatConfirmee::statut() const { return StatutCommande::CONFIRMEE; }
std::string EtatConfirmee::nom() const { return "CONFIRMEE"; }

// ---------- EtatPreparation ----------
void EtatPreparation::avancer(Commande& contexte) {
    contexte.definirEtat(std::make_unique<EtatColisPret>());
}
void EtatPreparation::annuler(Commande& contexte) {
    contexte.definirEtat(std::make_unique<EtatAnnulee>());
}
void EtatPreparation::declarerIncident(Commande& contexte) {
    contexte.definirEtat(std::make_unique<EtatIncident>());
}
StatutCommande EtatPreparation::statut() const { return StatutCommande::PREPARATION; }
std::string EtatPreparation::nom() const { return "PREPARATION"; }

// ---------- EtatColisPret ----------
void EtatColisPret::avancer(Commande& contexte) {
    contexte.definirEtat(std::make_unique<EtatAffectationVehicule>());
}
void EtatColisPret::annuler(Commande& contexte) {
    contexte.definirEtat(std::make_unique<EtatAnnulee>());
}
void EtatColisPret::declarerIncident(Commande& contexte) {
    contexte.definirEtat(std::make_unique<EtatIncident>());
}
StatutCommande EtatColisPret::statut() const { return StatutCommande::COLIS_PRET; }
std::string EtatColisPret::nom() const { return "COLIS_PRET"; }

// ---------- EtatAffectationVehicule ----------
void EtatAffectationVehicule::avancer(Commande& contexte) {
    contexte.definirEtat(std::make_unique<EtatExpedition>());
}
void EtatAffectationVehicule::annuler(Commande& contexte) {
    contexte.definirEtat(std::make_unique<EtatAnnulee>());
}
void EtatAffectationVehicule::declarerIncident(Commande& contexte) {
    contexte.definirEtat(std::make_unique<EtatIncident>());
}
StatutCommande EtatAffectationVehicule::statut() const { return StatutCommande::AFFECTATION_VEHICULE; }
std::string EtatAffectationVehicule::nom() const { return "AFFECTATION_VEHICULE"; }

// ---------- EtatExpedition ----------
void EtatExpedition::avancer(Commande& contexte) {
    contexte.definirEtat(std::make_unique<EtatEnTransit>());
}
void EtatExpedition::annuler(Commande&) {
    throw TransitionInvalideException("EXPEDITION", "annuler");
}
void EtatExpedition::declarerIncident(Commande& contexte) {
    contexte.definirEtat(std::make_unique<EtatIncident>());
}
StatutCommande EtatExpedition::statut() const { return StatutCommande::EXPEDITION; }
std::string EtatExpedition::nom() const { return "EXPEDITION"; }

// ---------- EtatEnTransit ----------
void EtatEnTransit::avancer(Commande& contexte) {
    contexte.definirEtat(std::make_unique<EtatEnLivraison>());
}
void EtatEnTransit::annuler(Commande&) {
    throw TransitionInvalideException("EN_TRANSIT", "annuler");
}
void EtatEnTransit::declarerIncident(Commande& contexte) {
    contexte.definirEtat(std::make_unique<EtatIncident>());
}
StatutCommande EtatEnTransit::statut() const { return StatutCommande::EN_TRANSIT; }
std::string EtatEnTransit::nom() const { return "EN_TRANSIT"; }

// ---------- EtatEnLivraison ----------
void EtatEnLivraison::avancer(Commande& contexte) {
    contexte.definirEtat(std::make_unique<EtatLivree>());
}
void EtatEnLivraison::annuler(Commande&) {
    throw TransitionInvalideException("EN_LIVRAISON", "annuler");
}
void EtatEnLivraison::declarerIncident(Commande& contexte) {
    contexte.definirEtat(std::make_unique<EtatIncident>());
}
StatutCommande EtatEnLivraison::statut() const { return StatutCommande::EN_LIVRAISON; }
std::string EtatEnLivraison::nom() const { return "EN_LIVRAISON"; }

// ---------- EtatLivree (terminal) ----------
void EtatLivree::avancer(Commande&) {
    throw TransitionInvalideException("LIVREE", "avancer");
}
void EtatLivree::annuler(Commande&) {
    throw TransitionInvalideException("LIVREE", "annuler");
}
void EtatLivree::declarerIncident(Commande&) {
    throw TransitionInvalideException("LIVREE", "declarerIncident");
}
StatutCommande EtatLivree::statut() const { return StatutCommande::LIVREE; }
std::string EtatLivree::nom() const { return "LIVREE"; }
bool EtatLivree::estTerminal() const { return true; }

// ---------- EtatAnnulee (terminal) ----------
void EtatAnnulee::avancer(Commande&) {
    throw TransitionInvalideException("ANNULEE", "avancer");
}
void EtatAnnulee::annuler(Commande&) {
    throw TransitionInvalideException("ANNULEE", "annuler");
}
void EtatAnnulee::declarerIncident(Commande&) {
    throw TransitionInvalideException("ANNULEE", "declarerIncident");
}
StatutCommande EtatAnnulee::statut() const { return StatutCommande::ANNULEE; }
std::string EtatAnnulee::nom() const { return "ANNULEE"; }
bool EtatAnnulee::estTerminal() const { return true; }

// ---------- EtatIncident ----------
void EtatIncident::avancer(Commande& contexte) {
    // Une fois l'incident jugé résolu, on reprend le transit
    // (le service de recalcul d'itinéraire aura fait son travail avant).
    contexte.definirEtat(std::make_unique<EtatEnTransit>());
}
void EtatIncident::annuler(Commande& contexte) {
    contexte.definirEtat(std::make_unique<EtatAnnulee>());
}
void EtatIncident::declarerIncident(Commande&) {
    throw TransitionInvalideException("INCIDENT", "declarerIncident");
}
StatutCommande EtatIncident::statut() const { return StatutCommande::INCIDENT; }
std::string EtatIncident::nom() const { return "INCIDENT"; }

} // namespace sl
