#include "persistence/PersistanceJSON.hpp"
#include "entities/VehiculeFrigorifique.hpp"
#include "factories/VehiculeFactory.hpp"
#include "utils/Exceptions.hpp"
#include <fstream>

namespace sl {

namespace {

// Codes courts et stables pour la persistance (independants de toString(),
// qui est destine a l'affichage humain et peut contenir des espaces/parentheses).
std::string codePermis(TypePermis type) {
    switch (type) {
        case TypePermis::B:    return "B";
        case TypePermis::C:    return "C";
        case TypePermis::C1:   return "C1";
        case TypePermis::MOTO: return "MOTO";
    }
    return "B";
}

TypePermis permisDepuisCode(const std::string& code) {
    if (code == "C")    return TypePermis::C;
    if (code == "C1")   return TypePermis::C1;
    if (code == "MOTO") return TypePermis::MOTO;
    return TypePermis::B;
}

} // namespace

// ---------- Client ----------

nlohmann::json PersistanceJSON::versJson(const Client& c) {
    return nlohmann::json{
        {"id", c.getId()},
        {"nom", c.getNom()},
        {"telephone", c.getTelephone()},
        {"adresse", c.getAdresse()}
    };
}

Client PersistanceJSON::clientDepuisJson(const nlohmann::json& j) {
    return Client(j.at("id").get<std::string>(),
                  j.at("nom").get<std::string>(),
                  j.at("telephone").get<std::string>(),
                  j.at("adresse").get<std::string>());
}

void PersistanceJSON::sauvegarderClients(const std::string& chemin, ClientRepository& repo) {
    nlohmann::json tableau = nlohmann::json::array();
    for (const Client* c : repo.tousLesElements()) tableau.push_back(versJson(*c));

    std::ofstream fichier(chemin);
    if (!fichier) throw PersistanceException(chemin);
    fichier << tableau.dump(2);
}

void PersistanceJSON::chargerClients(const std::string& chemin, ClientRepository& repo) {
    std::ifstream fichier(chemin);
    if (!fichier) throw PersistanceException(chemin);

    nlohmann::json tableau;
    fichier >> tableau;
    for (const auto& j : tableau) {
        repo.ajouter(std::make_unique<Client>(clientDepuisJson(j)));
    }
}

// ---------- Chauffeur ----------

nlohmann::json PersistanceJSON::versJson(const Chauffeur& c) {
    return nlohmann::json{
        {"id", c.getId()},
        {"nom", c.getNom()},
        {"permis", codePermis(c.getTypePermis())},
        {"experienceAnnees", c.getExperienceAnnees()},
        {"statut", static_cast<int>(c.getStatut())}
    };
}

Chauffeur PersistanceJSON::chauffeurDepuisJson(const nlohmann::json& j) {
    Chauffeur chauffeur(j.at("id").get<std::string>(),
                        j.at("nom").get<std::string>(),
                        permisDepuisCode(j.at("permis").get<std::string>()),
                        j.at("experienceAnnees").get<int>());
    if (j.contains("statut")) {
        chauffeur.setStatut(static_cast<StatutChauffeur>(j.at("statut").get<int>()));
    }
    return chauffeur;
}

void PersistanceJSON::sauvegarderChauffeurs(const std::string& chemin, ChauffeurRepository& repo) {
    nlohmann::json tableau = nlohmann::json::array();
    for (const Chauffeur* c : repo.tousLesElements()) tableau.push_back(versJson(*c));

    std::ofstream fichier(chemin);
    if (!fichier) throw PersistanceException(chemin);
    fichier << tableau.dump(2);
}

void PersistanceJSON::chargerChauffeurs(const std::string& chemin, ChauffeurRepository& repo) {
    std::ifstream fichier(chemin);
    if (!fichier) throw PersistanceException(chemin);

    nlohmann::json tableau;
    fichier >> tableau;
    for (const auto& j : tableau) {
        repo.ajouter(std::make_unique<Chauffeur>(chauffeurDepuisJson(j)));
    }
}

// ---------- Vehicule (polymorphe) ----------

nlohmann::json PersistanceJSON::versJson(const Vehicule& v) {
    nlohmann::json j{
        {"type", v.getType()},
        {"immatriculation", v.getImmatriculation()},
        {"marque", v.getMarque()},
        {"modele", v.getModele()},
        {"capaciteKg", v.getCapaciteKg()},
        {"kilometrage", v.getKilometrage()},
        {"carburantPct", v.getNiveauCarburantPct()},
        {"statut", static_cast<int>(v.getStatut())}
    };
    if (v.estFrigorifique()) {
        const auto& vf = static_cast<const VehiculeFrigorifique&>(v);
        j["temperatureConsigneC"] = vf.getTemperatureConsigneC();
    }
    return j;
}

std::unique_ptr<Vehicule> PersistanceJSON::vehiculeDepuisJson(const nlohmann::json& j) {
    ParametresVehicule p;
    p.immatriculation = j.at("immatriculation").get<std::string>();
    p.marque = j.at("marque").get<std::string>();
    p.modele = j.at("modele").get<std::string>();
    p.capaciteKg = j.at("capaciteKg").get<double>();
    p.kilometrage = j.value("kilometrage", 0.0);
    p.niveauCarburantPct = j.value("carburantPct", 100.0);
    p.temperatureConsigneC = j.value("temperatureConsigneC", 4.0);

    auto vehicule = VehiculeFactory::creer(j.at("type").get<std::string>(), p);
    if (j.contains("statut")) {
        vehicule->setStatut(static_cast<StatutVehicule>(j.at("statut").get<int>()));
    }
    return vehicule;
}

void PersistanceJSON::sauvegarderFlotte(const std::string& chemin, GestionnaireFlotte& flotte) {
    nlohmann::json tableau = nlohmann::json::array();
    for (Vehicule* v : flotte.tousLesVehicules()) tableau.push_back(versJson(*v));

    std::ofstream fichier(chemin);
    if (!fichier) throw PersistanceException(chemin);
    fichier << tableau.dump(2);
}

void PersistanceJSON::chargerFlotte(const std::string& chemin, GestionnaireFlotte& flotte) {
    std::ifstream fichier(chemin);
    if (!fichier) throw PersistanceException(chemin);

    nlohmann::json tableau;
    fichier >> tableau;
    for (const auto& j : tableau) {
        flotte.ajouterVehicule(vehiculeDepuisJson(j));
    }
}

} // namespace sl
