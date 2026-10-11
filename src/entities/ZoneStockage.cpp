#include "entities/ZoneStockage.hpp"
#include <algorithm>
#include <iostream>

namespace sl {

ZoneStockage::ZoneStockage(std::string id, std::string nom, double capaciteKg)
    : id_(std::move(id)), nom_(std::move(nom)), capaciteKg_(capaciteKg),
      chargeActuelleKg_(0.0) {}

const std::string& ZoneStockage::getId() const { return id_; }
const std::string& ZoneStockage::getNom() const { return nom_; }
double ZoneStockage::getCapaciteKg() const { return capaciteKg_; }
double ZoneStockage::getChargeActuelleKg() const { return chargeActuelleKg_; }
double ZoneStockage::getCapaciteRestanteKg() const { return capaciteKg_ - chargeActuelleKg_; }
const std::vector<std::string>& ZoneStockage::getColisIds() const { return colisIds_; }

bool ZoneStockage::ajouterColis(const std::string& colisId, double poidsKg) {
    if (poidsKg > getCapaciteRestanteKg()) return false;
    colisIds_.push_back(colisId);
    chargeActuelleKg_ += poidsKg;
    return true;
}

bool ZoneStockage::retirerColis(const std::string& colisId, double poidsKg) {
    auto it = std::find(colisIds_.begin(), colisIds_.end(), colisId);
    if (it == colisIds_.end()) return false;
    colisIds_.erase(it);
    chargeActuelleKg_ = std::max(0.0, chargeActuelleKg_ - poidsKg);
    return true;
}

double ZoneStockage::tauxRemplissage() const {
    if (capaciteKg_ <= 0.0) return 0.0;
    return chargeActuelleKg_ / capaciteKg_;
}

void ZoneStockage::afficherInformations() const {
    std::cout << "  Zone " << nom_ << " (" << id_ << ") : "
               << chargeActuelleKg_ << "/" << capaciteKg_ << " kg ("
               << (tauxRemplissage() * 100.0) << "%), "
               << colisIds_.size() << " colis\n";
}

} // namespace sl
