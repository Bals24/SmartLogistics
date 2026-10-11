#include "entities/Entrepot.hpp"
#include <iostream>
#include <numeric>

namespace sl {

Entrepot::Entrepot(std::string id, std::string nom, std::string ville)
    : id_(std::move(id)), nom_(std::move(nom)), ville_(std::move(ville)) {}

const std::string& Entrepot::getId() const { return id_; }
const std::string& Entrepot::getNom() const { return nom_; }
const std::string& Entrepot::getVille() const { return ville_; }

void Entrepot::ajouterZone(std::unique_ptr<ZoneStockage> zone) {
    zones_.push_back(std::move(zone));
}

const std::vector<std::unique_ptr<ZoneStockage>>& Entrepot::getZones() const { return zones_; }

double Entrepot::capaciteTotaleKg() const {
    return std::accumulate(zones_.begin(), zones_.end(), 0.0,
        [](double s, const std::unique_ptr<ZoneStockage>& z) { return s + z->getCapaciteKg(); });
}

double Entrepot::chargeActuelleTotaleKg() const {
    return std::accumulate(zones_.begin(), zones_.end(), 0.0,
        [](double s, const std::unique_ptr<ZoneStockage>& z) { return s + z->getChargeActuelleKg(); });
}

double Entrepot::tauxRemplissageGlobal() const {
    double capacite = capaciteTotaleKg();
    if (capacite <= 0.0) return 0.0;
    return chargeActuelleTotaleKg() / capacite;
}

ZoneStockage* Entrepot::trouverZoneDisponible(double poidsKg) {
    for (auto& zone : zones_) {
        if (zone->getCapaciteRestanteKg() >= poidsKg) return zone.get();
    }
    return nullptr;
}

bool Entrepot::estPresqueSature(double seuil) const {
    return tauxRemplissageGlobal() >= seuil;
}

void Entrepot::afficherInformations() const {
    std::cout << "------------------------------\n";
    std::cout << "ENTREPOT " << nom_ << " (" << id_ << ") - " << ville_ << "\n";
    std::cout << "Capacite totale : " << capaciteTotaleKg() << " kg\n";
    std::cout << "Charge actuelle : " << chargeActuelleTotaleKg() << " kg ("
               << (tauxRemplissageGlobal() * 100.0) << "%)\n";
    for (const auto& zone : zones_) {
        zone->afficherInformations();
    }
    if (estPresqueSature()) {
        std::cout << "  /!\\ STOCKAGE PRESQUE SATURE\n";
    }
    std::cout << "------------------------------\n";
}

} // namespace sl
