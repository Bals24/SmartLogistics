#ifndef SMARTLOGISTICS_REPOSITORIES_HPP
#define SMARTLOGISTICS_REPOSITORIES_HPP

#include "repositories/RepositoryMemoire.hpp"
#include "entities/Client.hpp"
#include "entities/Chauffeur.hpp"
#include "entities/Vehicule.hpp"
#include "entities/Commande.hpp"
#include "entities/Livraison.hpp"
#include "entities/Entrepot.hpp"

namespace sl {

using ClientRepository = RepositoryMemoire<Client>;
using ChauffeurRepository = RepositoryMemoire<Chauffeur>;
using VehiculeRepository = RepositoryMemoire<Vehicule>;      // polymorphe (Camion, Fourgon, ...)
using CommandeRepository = RepositoryMemoire<Commande>;
using LivraisonRepository = RepositoryMemoire<Livraison>;
using EntrepotRepository = RepositoryMemoire<Entrepot>;

} // namespace sl

#endif // SMARTLOGISTICS_REPOSITORIES_HPP
