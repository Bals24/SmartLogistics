#ifndef SMARTLOGISTICS_ENTREPOT_HPP
#define SMARTLOGISTICS_ENTREPOT_HPP

#include <string>
#include <vector>
#include <memory>
#include "entities/ZoneStockage.hpp"

namespace sl {

// Un Entrepot POSSEDE ses zones de stockage (composition : la durée de vie
// des zones est liée à celle de l'entrepôt).
class Entrepot {
public:
    Entrepot(std::string id, std::string nom, std::string ville);

    const std::string& getId() const;
    const std::string& getNom() const;
    const std::string& getVille() const;

    void ajouterZone(std::unique_ptr<ZoneStockage> zone);
    const std::vector<std::unique_ptr<ZoneStockage>>& getZones() const;

    double capaciteTotaleKg() const;
    double chargeActuelleTotaleKg() const;
    double tauxRemplissageGlobal() const;

    // Cherche la première zone ayant assez de place ; nullptr si aucune.
    ZoneStockage* trouverZoneDisponible(double poidsKg);

    // Seuil d'alerte : entrepôt considéré "presque saturé" au-delà.
    bool estPresqueSature(double seuil = 0.9) const;

    void afficherInformations() const;

private:
    std::string id_;
    std::string nom_;
    std::string ville_;
    std::vector<std::unique_ptr<ZoneStockage>> zones_;
};

} // namespace sl

#endif // SMARTLOGISTICS_ENTREPOT_HPP
