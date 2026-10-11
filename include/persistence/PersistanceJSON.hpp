#ifndef SMARTLOGISTICS_PERSISTANCEJSON_HPP
#define SMARTLOGISTICS_PERSISTANCEJSON_HPP

#include <string>
#include <memory>
#include "external/json.hpp"
#include "entities/Client.hpp"
#include "entities/Chauffeur.hpp"
#include "entities/Vehicule.hpp"
#include "repositories/Repositories.hpp"
#include "services/GestionnaireFlotte.hpp"

namespace sl {

// Persistance JSON (Niveau 4, phase 1). Chaque fonction sauvegarderXxx /
// chargerXxx leve PersistanceException si le fichier ne peut pas etre
// ouvert. Les conversions unitaires versJson/xxxDepuisJson sont exposees
// publiquement pour pouvoir etre testees isolement (voir tests/).
class PersistanceJSON {
public:
    static void sauvegarderClients(const std::string& chemin, ClientRepository& repo);
    static void chargerClients(const std::string& chemin, ClientRepository& repo);

    static void sauvegarderChauffeurs(const std::string& chemin, ChauffeurRepository& repo);
    static void chargerChauffeurs(const std::string& chemin, ChauffeurRepository& repo);

    static void sauvegarderFlotte(const std::string& chemin, GestionnaireFlotte& flotte);
    static void chargerFlotte(const std::string& chemin, GestionnaireFlotte& flotte);

    static nlohmann::json versJson(const Client& client);
    static Client clientDepuisJson(const nlohmann::json& j);

    static nlohmann::json versJson(const Chauffeur& chauffeur);
    static Chauffeur chauffeurDepuisJson(const nlohmann::json& j);

    static nlohmann::json versJson(const Vehicule& vehicule);
    static std::unique_ptr<Vehicule> vehiculeDepuisJson(const nlohmann::json& j);
};

} // namespace sl

#endif // SMARTLOGISTICS_PERSISTANCEJSON_HPP
