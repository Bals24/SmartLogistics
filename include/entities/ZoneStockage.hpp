#ifndef SMARTLOGISTICS_ZONESTOCKAGE_HPP
#define SMARTLOGISTICS_ZONESTOCKAGE_HPP

#include <string>
#include <vector>

namespace sl {

class ZoneStockage {
public:
    ZoneStockage(std::string id, std::string nom, double capaciteKg);

    const std::string& getId() const;
    const std::string& getNom() const;
    double getCapaciteKg() const;
    double getChargeActuelleKg() const;
    double getCapaciteRestanteKg() const;
    const std::vector<std::string>& getColisIds() const;

    // Ajoute un colis (par id) si la capacité le permet ; renvoie false sinon.
    bool ajouterColis(const std::string& colisId, double poidsKg);
    bool retirerColis(const std::string& colisId, double poidsKg);

    double tauxRemplissage() const; // entre 0.0 et 1.0

    void afficherInformations() const;

private:
    std::string id_;
    std::string nom_;
    double capaciteKg_;
    double chargeActuelleKg_;
    std::vector<std::string> colisIds_;
};

} // namespace sl

#endif // SMARTLOGISTICS_ZONESTOCKAGE_HPP
