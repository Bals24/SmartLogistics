#include "observateurs/ObservateursConcrets.hpp"
#include "entities/Commande.hpp"
#include <iostream>

namespace sl {

void NotificationClient::notifier(const Commande& commande,
                                   StatutCommande ancienStatut,
                                   StatutCommande nouveauStatut) {
    const Client* client = commande.getClient();
    std::cout << "[NOTIFICATION CLIENT] ";
    if (client) std::cout << client->getNom() << " : ";
    std::cout << "votre commande " << commande.getId() << " passe de "
               << toString(ancienStatut) << " a " << toString(nouveauStatut) << "\n";
}

void NotificationGestionnaire::notifier(const Commande& commande,
                                         StatutCommande ancienStatut,
                                         StatutCommande nouveauStatut) {
    std::cout << "[NOTIFICATION GESTIONNAIRE] Commande " << commande.getId()
               << " : " << toString(ancienStatut) << " -> " << toString(nouveauStatut) << "\n";
}

} // namespace sl
