#ifndef SMARTLOGISTICS_OBSERVATEURSCONCRETS_HPP
#define SMARTLOGISTICS_OBSERVATEURSCONCRETS_HPP

#include "observateurs/IObservateurCommande.hpp"

namespace sl {

// Notifie le client final (affichage console pour la démonstration ;
// pourrait être remplacé par un envoi SMS/email dans une vraie appli).
class NotificationClient : public IObservateurCommande {
public:
    void notifier(const Commande& commande,
                  StatutCommande ancienStatut,
                  StatutCommande nouveauStatut) override;
};

// Notifie le gestionnaire logistique (tableau de bord interne).
class NotificationGestionnaire : public IObservateurCommande {
public:
    void notifier(const Commande& commande,
                  StatutCommande ancienStatut,
                  StatutCommande nouveauStatut) override;
};

} // namespace sl

#endif // SMARTLOGISTICS_OBSERVATEURSCONCRETS_HPP
