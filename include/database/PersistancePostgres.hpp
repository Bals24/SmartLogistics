#ifndef SMARTLOGISTICS_PERSISTANCEPOSTGRES_HPP
#define SMARTLOGISTICS_PERSISTANCEPOSTGRES_HPP

#include <cstddef>
#include "database/Database.hpp"
#include "repositories/Repositories.hpp"
#include "services/GestionnaireFlotte.hpp"

namespace sl {

struct ResumeChargement {
    std::size_t ajoutes = 0;
    std::size_t ignores = 0;
    ResumeChargement& operator+=(const ResumeChargement& autre) {
        ajoutes += autre.ajoutes;
        ignores += autre.ignores;
        return *this;
    }
};

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