#ifndef SMARTLOGISTICS_VEHICULE_HPP
#define SMARTLOGISTICS_VEHICULE_HPP

#include <string>
#include "entities/Enums.hpp"

namespace sl {

// Classe abstraite représentant un véhicule de la flotte.
// Les sous-classes (Camion, Fourgon, Moto, VehiculeFrigorifique)
// définissent leur propre capacité de transport et leurs contraintes.
class Vehicule {
public:
    Vehicule(std::string immatriculation,
             std::string marque,
             std::string modele,
             double capaciteKg,
             double kilometrage = 0.0,
             double niveauCarburantPct = 100.0);

    virtual ~Vehicule() = default;

    // --- Accesseurs ---
    const std::string& getImmatriculation() const;
    const std::string& getMarque() const;
    const std::string& getModele() const;
    double getCapaciteKg() const;
    double getChargeActuelleKg() const;
    double getKilometrage() const;
    double getNiveauCarburantPct() const;
    StatutVehicule getStatut() const;

    // --- Mutateurs contrôlés (encapsulation) ---
    void setStatut(StatutVehicule statut);
    void ajouterKilometrage(double km);
    void definirNiveauCarburant(double pct);

    // Tente de charger un poids supplémentaire. Renvoie false si la
    // capacité serait dépassée (règle métier simple, sans lever d'exception).
    bool chargerPoids(double poidsKg);
    void viderChargement();

    double getCapaciteRestanteKg() const;

    // --- Comportement polymorphe ---
    virtual void afficherInformations() const;
    virtual std::string getType() const = 0;

    // Un véhicule frigorifique redéfinira ceci à true.
    virtual bool estFrigorifique() const;

    // Type de permis minimal requis pour conduire ce véhicule
    // (utilisé par la règle métier "permis <-> véhicule", Niveau 2).
    virtual TypePermis permisRequis() const = 0;

protected:
    void afficherEntete() const;

private:
    std::string immatriculation_;
    std::string marque_;
    std::string modele_;
    double capaciteKg_;
    double chargeActuelleKg_;
    double kilometrage_;
    double niveauCarburantPct_;
    StatutVehicule statut_;
};

} // namespace sl

#endif // SMARTLOGISTICS_VEHICULE_HPP
