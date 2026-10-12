#ifndef SMARTLOGISTICS_PERSISTANCEPOSTGRES_HPP
#define SMARTLOGISTICS_PERSISTANCEPOSTGRES_HPP

#include <cstddef>

#include "database/Database.hpp"
#include "repositories/Repositories.hpp"
#include "services/GestionnaireFlotte.hpp"

namespace sl {

// Resultat d'un chargement depuis PostgreSQL.
struct ResumeChargement {
    std::size_t ajoutes = 0;   // elements ajoutes en memoire
    std::size_t ignores = 0;   // deja en memoire, ou type inconnu

    ResumeChargement& operator+=(const ResumeChargement& autre) {
        ajoutes += autre.ajoutes;
        ignores += autre.ignores;
        return *this;
    }
};

// Persistance PostgreSQL des clients, chauffeurs et vehicules.
// Meme esprit que PersistanceJSON : les objets metier ne savent rien
// de la base, seule cette classe traduit objets <-> lignes SQL.
//
// - sauvegarder* : "upsert" (INSERT ou UPDATE) dans UNE transaction.
// - charger*     : ajoute en memoire les lignes absentes ; ne remplace
//                  jamais un objet deja present.
// - Toutes les fonctions lèvent SmartLogisticsException en cas d'erreur
//   SQL (la transaction est alors annulee).
// - Les colonnes manquantes (code, experience_annees, ...) sont creees
//   automatiquement sur les tables de bdd.txt (idempotent).
class PersistancePostgres {
public:
    static void sauvegarderClients(SmartLogistics::Database& db, ClientRepository& repo);
    static ResumeChargement chargerClients(SmartLogistics::Database& db, ClientRepository& repo);

    static void sauvegarderChauffeurs(SmartLogistics::Database& db, ChauffeurRepository& repo);
    static ResumeChargement chargerChauffeurs(SmartLogistics::Database& db, ChauffeurRepository& repo);

    static void sauvegarderFlotte(SmartLogistics::Database& db, GestionnaireFlotte& flotte);
    static ResumeChargement chargerFlotte(SmartLogistics::Database& db, GestionnaireFlotte& flotte);

    static void sauvegarderTout(SmartLogistics::Database& db,
                                ClientRepository& clients,
                                ChauffeurRepository& chauffeurs,
                                GestionnaireFlotte& flotte);

    static ResumeChargement chargerTout(SmartLogistics::Database& db,
                                        ClientRepository& clients,
                                        ChauffeurRepository& chauffeurs,
                                        GestionnaireFlotte& flotte);
};

} // namespace sl

#endif // SMARTLOGISTICS_PERSISTANCEPOSTGRES_HPP