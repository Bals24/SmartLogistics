#ifndef SMARTLOGISTICS_INCIDENTSCONCRETS_HPP
#define SMARTLOGISTICS_INCIDENTSCONCRETS_HPP

#include "entities/Incident.hpp"

namespace sl {

// Un accident bloque toujours la livraison et est au minimum de gravité ELEVEE.
class Accident : public Incident {
public:
    Accident(std::string id, std::string description, std::string localisation,
             std::string livraisonId, GraviteIncident gravite = GraviteIncident::ELEVEE);

    std::string getType() const override;
    bool bloqueLivraison() const override;
};

// Une panne mécanique du véhicule.
class Panne : public Incident {
public:
    Panne(std::string id, std::string description, std::string localisation,
          std::string livraisonId, std::string typePanne,
          GraviteIncident gravite = GraviteIncident::MOYENNE);

    std::string getType() const override;
    bool bloqueLivraison() const override;
    void afficherInformations() const override;

    const std::string& getTypePanne() const;

private:
    std::string typePanne_; // ex: "mecanique", "carburant", "pneu creve"
};

// Une route est bloquée : nécessite un recalcul d'itinéraire.
class RouteBloquee : public Incident {
public:
    RouteBloquee(std::string id, std::string description, std::string localisation,
                 std::string livraisonId, GraviteIncident gravite = GraviteIncident::ELEVEE);

    std::string getType() const override;
    bool bloqueLivraison() const override;
};

// Un retard estimé, sans blocage complet de la livraison.
class Retard : public Incident {
public:
    Retard(std::string id, std::string description, std::string localisation,
           std::string livraisonId, int retardEstimeMinutes,
           GraviteIncident gravite = GraviteIncident::FAIBLE);

    std::string getType() const override;
    bool bloqueLivraison() const override;
    void afficherInformations() const override;

    int getRetardEstimeMinutes() const;

private:
    int retardEstimeMinutes_;
};

// Un colis a été endommagé pendant le transport : ne bloque pas la route.
class ColisEndommage : public Incident {
public:
    ColisEndommage(std::string id, std::string description, std::string localisation,
                   std::string livraisonId, std::string colisId,
                   GraviteIncident gravite = GraviteIncident::MOYENNE);

    std::string getType() const override;
    bool bloqueLivraison() const override;
    void afficherInformations() const override;

    const std::string& getColisId() const;

private:
    std::string colisId_;
};

} // namespace sl

#endif // SMARTLOGISTICS_INCIDENTSCONCRETS_HPP
