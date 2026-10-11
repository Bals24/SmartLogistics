#ifndef SMARTLOGISTICS_ETATCOMMANDE_HPP
#define SMARTLOGISTICS_ETATCOMMANDE_HPP

#include <string>
#include <memory>
#include "entities/Enums.hpp"

namespace sl {

class Commande; // forward declaration : évite l'inclusion circulaire Commande <-> EtatCommande

// Interface du State Pattern pour le cycle de vie d'une Commande.
// Chaque état concret sait dans quel(s) état(s) suivant(s) il peut transiter
// et se charge lui-même de faire évoluer le contexte (Commande).
class EtatCommande {
public:
    virtual ~EtatCommande() = default;

    // Fait avancer la commande à l'étape suivante du cycle normal.
    // Lève TransitionInvalideException si l'état est terminal.
    virtual void avancer(Commande& contexte) = 0;

    // Annule la commande. Valide depuis tout état non terminal.
    virtual void annuler(Commande& contexte) = 0;

    // Déclare un incident sur la commande (ex: colis endommagé, route bloquée).
    virtual void declarerIncident(Commande& contexte) = 0;

    virtual StatutCommande statut() const = 0;
    virtual std::string nom() const = 0;

    // Un état terminal ne peut plus évoluer normalement.
    virtual bool estTerminal() const { return false; }
};

} // namespace sl

#endif // SMARTLOGISTICS_ETATCOMMANDE_HPP
