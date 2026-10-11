#ifndef SMARTLOGISTICS_INCIDENT_HPP
#define SMARTLOGISTICS_INCIDENT_HPP

#include <string>
#include <ctime>
#include "entities/Enums.hpp"

namespace sl {

// Classe abstraite représentant un incident logistique. Les sous-classes
// concrètes (Accident, Panne, RouteBloquee, Retard, ColisEndommage)
// définissent leur propre gravité par défaut et leur affichage.
class Incident {
public:
    Incident(std::string id,
             std::string description,
             std::string localisation,
             std::string livraisonId,
             GraviteIncident gravite);

    virtual ~Incident() = default;

    const std::string& getId() const;
    const std::string& getDescription() const;
    const std::string& getLocalisation() const;
    const std::string& getLivraisonId() const;
    GraviteIncident getGravite() const;
    std::time_t getDateCreation() const;

    virtual std::string getType() const = 0;

    // Indique si l'incident bloque la progression de la livraison
    // (ex : route bloquée = oui, colis endommagé = non).
    virtual bool bloqueLivraison() const;

    virtual void afficherInformations() const;

protected:
    void afficherEntete() const;

private:
    std::string id_;
    std::string description_;
    std::string localisation_;
    std::string livraisonId_;
    GraviteIncident gravite_;
    std::time_t dateCreation_;
};

} // namespace sl

#endif // SMARTLOGISTICS_INCIDENT_HPP
