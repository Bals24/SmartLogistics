#ifndef SMARTLOGISTICS_COLIS_HPP
#define SMARTLOGISTICS_COLIS_HPP

#include <string>
#include <memory>
#include "entities/Enums.hpp"

namespace sl {

// Classe abstraite représentant un colis.
// Toute sous-classe concrète (ColisStandard, ColisFragile, ColisExpress,
// ColisRefrigere) doit fournir son propre comportement d'affichage
// et ses éventuelles contraintes de transport (polymorphisme).
class Colis {
public:
    Colis(std::string id,
          std::string description,
          double poidsKg,
          double volumeM3,
          std::string origine,
          std::string destination,
          Priorite priorite = Priorite::STANDARD);

    virtual ~Colis() = default;

    // --- Accesseurs (encapsulation : pas d'attribut public) ---
    const std::string& getId() const;
    const std::string& getDescription() const;
    double getPoidsKg() const;
    double getVolumeM3() const;
    const std::string& getOrigine() const;
    const std::string& getDestination() const;
    Priorite getPriorite() const;
    StatutColis getStatut() const;

    // --- Mutateurs contrôlés ---
    void setStatut(StatutColis nouveauStatut);

    // --- Comportement polymorphe ---
    // Chaque type concret de colis affiche ses informations avec ses
    // propres particularités (ex : température pour le réfrigéré).
    virtual void afficherInformations() const;

    // Renvoie une étiquette du type concret ("STANDARD", "FRAGILE", ...)
    // utilisée notamment par la VehicleFactory / les règles métier futures.
    virtual std::string getType() const = 0;

    // Certains colis imposent des contraintes de transport
    // (ex : un colis réfrigéré nécessite un véhicule frigorifique).
    // Par défaut, aucune contrainte particulière.
    virtual bool necessiteVehiculeSpecial() const;

protected:
    // Utilitaire protégé pour factoriser l'entête d'affichage
    // entre toutes les sous-classes.
    void afficherEntete() const;

private:
    std::string id_;
    std::string description_;
    double poidsKg_;
    double volumeM3_;
    std::string origine_;
    std::string destination_;
    Priorite priorite_;
    StatutColis statut_;
};

} // namespace sl

#endif // SMARTLOGISTICS_COLIS_HPP
