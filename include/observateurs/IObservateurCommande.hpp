#ifndef SMARTLOGISTICS_IOBSERVATEURCOMMANDE_HPP
#define SMARTLOGISTICS_IOBSERVATEURCOMMANDE_HPP

#include "entities/Enums.hpp"

namespace sl {

class Commande; // forward declaration

// Interface du Observer Pattern : tout objet intéressé par les changements
// de statut d'une commande (client, gestionnaire, chauffeur...) l'implémente
// et s'abonne via Commande::abonner().
class IObservateurCommande {
public:
    virtual ~IObservateurCommande() = default;

    virtual void notifier(const Commande& commande,
                           StatutCommande ancienStatut,
                           StatutCommande nouveauStatut) = 0;
};

} // namespace sl

#endif // SMARTLOGISTICS_IOBSERVATEURCOMMANDE_HPP
