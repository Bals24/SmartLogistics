#ifndef SMARTLOGISTICS_ETATSCOMMANDECONCRETS_HPP
#define SMARTLOGISTICS_ETATSCOMMANDECONCRETS_HPP

#include "etats/EtatCommande.hpp"

namespace sl {

// Macro-like helper pattern : chaque état ne fait qu'une chose, transiter.
// La notification aux observateurs est centralisée dans Commande::avancer()
// / annulerCommande() / declarerIncident(), pas ici.

class EtatCree : public EtatCommande {
public:
    void avancer(Commande& contexte) override;
    void annuler(Commande& contexte) override;
    void declarerIncident(Commande& contexte) override;
    StatutCommande statut() const override;
    std::string nom() const override;
};

class EtatConfirmee : public EtatCommande {
public:
    void avancer(Commande& contexte) override;
    void annuler(Commande& contexte) override;
    void declarerIncident(Commande& contexte) override;
    StatutCommande statut() const override;
    std::string nom() const override;
};

class EtatPreparation : public EtatCommande {
public:
    void avancer(Commande& contexte) override;
    void annuler(Commande& contexte) override;
    void declarerIncident(Commande& contexte) override;
    StatutCommande statut() const override;
    std::string nom() const override;
};

class EtatColisPret : public EtatCommande {
public:
    void avancer(Commande& contexte) override;
    void annuler(Commande& contexte) override;
    void declarerIncident(Commande& contexte) override;
    StatutCommande statut() const override;
    std::string nom() const override;
};

class EtatAffectationVehicule : public EtatCommande {
public:
    void avancer(Commande& contexte) override;
    void annuler(Commande& contexte) override;
    void declarerIncident(Commande& contexte) override;
    StatutCommande statut() const override;
    std::string nom() const override;
};

class EtatExpedition : public EtatCommande {
public:
    void avancer(Commande& contexte) override;
    void annuler(Commande& contexte) override; // lève une exception : trop tard pour annuler
    void declarerIncident(Commande& contexte) override;
    StatutCommande statut() const override;
    std::string nom() const override;
};

class EtatEnTransit : public EtatCommande {
public:
    void avancer(Commande& contexte) override;
    void annuler(Commande& contexte) override;
    void declarerIncident(Commande& contexte) override;
    StatutCommande statut() const override;
    std::string nom() const override;
};

class EtatEnLivraison : public EtatCommande {
public:
    void avancer(Commande& contexte) override;
    void annuler(Commande& contexte) override;
    void declarerIncident(Commande& contexte) override;
    StatutCommande statut() const override;
    std::string nom() const override;
};

class EtatLivree : public EtatCommande {
public:
    void avancer(Commande& contexte) override;    // lève une exception (terminal)
    void annuler(Commande& contexte) override;     // lève une exception (terminal)
    void declarerIncident(Commande& contexte) override; // lève une exception (terminal)
    StatutCommande statut() const override;
    std::string nom() const override;
    bool estTerminal() const override;
};

class EtatAnnulee : public EtatCommande {
public:
    void avancer(Commande& contexte) override;
    void annuler(Commande& contexte) override;
    void declarerIncident(Commande& contexte) override;
    StatutCommande statut() const override;
    std::string nom() const override;
    bool estTerminal() const override;
};

// État atteint quand un incident bloque la commande. On peut soit reprendre
// le transit (une fois l'incident résolu), soit annuler la commande.
class EtatIncident : public EtatCommande {
public:
    void avancer(Commande& contexte) override; // reprise -> EtatEnTransit
    void annuler(Commande& contexte) override;
    void declarerIncident(Commande& contexte) override; // déjà en incident
    StatutCommande statut() const override;
    std::string nom() const override;
};

} // namespace sl

#endif // SMARTLOGISTICS_ETATSCOMMANDECONCRETS_HPP
