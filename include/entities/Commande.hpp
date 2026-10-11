#ifndef SMARTLOGISTICS_COMMANDE_HPP
#define SMARTLOGISTICS_COMMANDE_HPP

#include <string>
#include <vector>
#include <memory>
#include "entities/Colis.hpp"
#include "entities/Client.hpp"
#include "entities/Enums.hpp"
#include "etats/EtatCommande.hpp"
#include "observateurs/IObservateurCommande.hpp"

namespace sl {

// Une Commande est COMPOSEE de Colis (elle possede et gere leur duree de vie
// via des unique_ptr) et ASSOCIEE a un Client (pointeur non possedant, le
// client existe independamment de la commande, typiquement dans un
// ClientRepository).
//
// Son cycle de vie est gere par le State Pattern (etat_) : c'est l'etat
// courant qui decide si une transition est possible et vers quel etat
// suivant. Chaque transition notifie les observateurs abonnes (Observer
// Pattern), par exemple le client ou le gestionnaire logistique.
class Commande {
public:
    Commande(std::string id,
             const Client* client,
             std::string destination,
             Priorite priorite = Priorite::STANDARD);

    Commande(const Commande&) = delete;
    Commande& operator=(const Commande&) = delete;
    Commande(Commande&&) = default;
    Commande& operator=(Commande&&) = default;

    const std::string& getId() const;
    const Client* getClient() const;
    const std::string& getDestination() const;
    Priorite getPriorite() const;

    // --- State Pattern : delegue a l'etat courant ---
    StatutCommande getStatut() const;
    std::string getNomEtat() const;
    void avancer();
    void annulerCommande();
    void declarerIncident();

    void definirEtat(std::unique_ptr<EtatCommande> nouvelEtat);

    // --- Observer Pattern ---
    void abonner(IObservateurCommande* observateur);
    void notifierObservateurs(StatutCommande ancien, StatutCommande nouveau) const;

    // --- Composition de colis ---
    void ajouterColis(std::unique_ptr<Colis> colis);
    const std::vector<std::unique_ptr<Colis>>& getColis() const;

    double poidsTotalKg() const;
    double volumeTotalM3() const;
    bool necessiteVehiculeSpecial() const;

    void afficherInformations() const;

private:
    std::string id_;
    const Client* client_;
    std::string destination_;
    Priorite priorite_;
    std::unique_ptr<EtatCommande> etat_;
    std::vector<std::unique_ptr<Colis>> colis_;
    std::vector<IObservateurCommande*> observateurs_;
};

} // namespace sl

#endif // SMARTLOGISTICS_COMMANDE_HPP
