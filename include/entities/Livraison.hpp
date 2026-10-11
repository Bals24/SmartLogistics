#ifndef SMARTLOGISTICS_LIVRAISON_HPP
#define SMARTLOGISTICS_LIVRAISON_HPP

#include <string>
#include <vector>
#include <memory>
#include "entities/Commande.hpp"
#include "entities/Vehicule.hpp"
#include "entities/Chauffeur.hpp"
#include "entities/Incident.hpp"
#include "graphe/Itineraire.hpp"
#include "entities/Enums.hpp"

namespace sl {

// Une Livraison est une mission logistique concrète. Elle est ASSOCIEE à une
// Commande, un Vehicule et un Chauffeur (tous gérés ailleurs, par exemple
// dans des repositories — d'où des pointeurs non possédants), mais elle
// POSSEDE son Itineraire calculé et ses Incidents (composition).
class Livraison {
public:
    Livraison(std::string id,
              Commande* commande,
              Vehicule* vehicule,
              Chauffeur* chauffeur,
              Itineraire itineraire);

    const std::string& getId() const;
    Commande* getCommande() const;
    Vehicule* getVehicule() const;
    Chauffeur* getChauffeur() const;
    const Itineraire& getItineraire() const;
    StatutLivraison getStatut() const;

    void setStatut(StatutLivraison statut);

    void ajouterIncident(std::unique_ptr<Incident> incident);
    const std::vector<std::unique_ptr<Incident>>& getIncidents() const;

    // Vrai si au moins un incident actif bloque la progression.
    bool estBloquee() const;

    void afficherInformations() const;

private:
    std::string id_;
    Commande* commande_;
    Vehicule* vehicule_;
    Chauffeur* chauffeur_;
    Itineraire itineraire_;
    StatutLivraison statut_;
    std::vector<std::unique_ptr<Incident>> incidents_;
};

} // namespace sl

#endif // SMARTLOGISTICS_LIVRAISON_HPP
