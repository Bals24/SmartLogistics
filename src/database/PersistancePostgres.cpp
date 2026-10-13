#include "database/PersistancePostgres.hpp"
#include <iomanip>
#include <locale>
#include <sstream>
#include <string>
#include <vector>
#include "entities/VehiculeFrigorifique.hpp"
#include "factories/VehiculeFactory.hpp"
#include "utils/Exceptions.hpp"

namespace sl {

using SmartLogistics::Database;
using SmartLogistics::QueryResult;
using SmartLogistics::SqlParam;

namespace {

std::string nombreVersTexte(double valeur) {
    std::ostringstream flux;
    flux.imbue(std::locale::classic());
    flux << std::fixed << std::setprecision(2) << valeur;
    return flux.str();
}

double texteVersNombre(const std::string& texte, double parDefaut) {
    std::istringstream flux(texte);
    flux.imbue(std::locale::classic());
    double valeur = 0.0;
    return (flux >> valeur) ? valeur : parDefaut;
}

int texteVersEntier(const std::string& texte, int parDefaut) {
    try { return std::stoi(texte); } catch (...) { return parDefaut; }
}

std::string codePermis(TypePermis type) {
    if (type == TypePermis::C) return "C";
    if (type == TypePermis::C1) return "C1";
    if (type == TypePermis::MOTO) return "MOTO";
    return "B";
}

TypePermis permisDepuisCode(const std::string& code) {
    if (code == "C") return TypePermis::C;
    if (code == "C1") return TypePermis::C1;
    if (code == "MOTO") return TypePermis::MOTO;
    return TypePermis::B;
}

std::string codeStatut(StatutVehicule statut) {
    if (statut == StatutVehicule::EN_MISSION) return "EN_MISSION";
    if (statut == StatutVehicule::EN_MAINTENANCE) return "EN_MAINTENANCE";
    if (statut == StatutVehicule::INDISPONIBLE) return "INDISPONIBLE";
    return "DISPONIBLE";
}

StatutVehicule statutVehiculeDepuisCode(const std::string& code) {
    if (code == "EN_MISSION") return StatutVehicule::EN_MISSION;
    if (code == "EN_MAINTENANCE") return StatutVehicule::EN_MAINTENANCE;
    if (code == "INDISPONIBLE") return StatutVehicule::INDISPONIBLE;
    return StatutVehicule::DISPONIBLE;
}

std::string codeStatut(StatutChauffeur statut) {
    if (statut == StatutChauffeur::EN_MISSION) return "EN_MISSION";
    if (statut == StatutChauffeur::EN_CONGE) return "EN_CONGE";
    if (statut == StatutChauffeur::INDISPONIBLE) return "INDISPONIBLE";
    return "DISPONIBLE";
}

StatutChauffeur statutChauffeurDepuisCode(const std::string& code) {
    if (code == "EN_MISSION") return StatutChauffeur::EN_MISSION;
    if (code == "EN_CONGE") return StatutChauffeur::EN_CONGE;
    if (code == "INDISPONIBLE") return StatutChauffeur::INDISPONIBLE;
    return StatutChauffeur::DISPONIBLE;
}

void executerOuLever(Database& db, const std::string& sql, const std::vector<SqlParam>& params = {}) {
    if (!db.execute(sql, params)) throw SmartLogisticsException("PostgreSQL : " + db.lastError());
}

QueryResult interrogerOuLever(Database& db, const std::string& sql) {
    QueryResult resultat = db.query(sql);
    if (!resultat.ok()) throw SmartLogisticsException("PostgreSQL : " + db.lastError());
    return resultat;
}

class Transaction {
public:
    explicit Transaction(Database& db) : db_(db) { executerOuLever(db_, "BEGIN"); }
    ~Transaction() { if (!terminee_) db_.execute("ROLLBACK"); }
    void valider() { executerOuLever(db_, "COMMIT"); terminee_ = true; }
    Transaction(const Transaction&) = delete;
    Transaction& operator=(const Transaction&) = delete;
private:
    Database& db_;
    bool terminee_ = false;
};

void preparerSchema(Database& db) {
    static const char* const instructions[] = {
        "ALTER TABLE clients ADD COLUMN IF NOT EXISTS code VARCHAR(30)",
        "CREATE UNIQUE INDEX IF NOT EXISTS ux_clients_code ON clients(code)",
        "ALTER TABLE chauffeurs ADD COLUMN IF NOT EXISTS code VARCHAR(30)",
        "ALTER TABLE chauffeurs ADD COLUMN IF NOT EXISTS experience_annees INTEGER NOT NULL DEFAULT 0",
        "CREATE UNIQUE INDEX IF NOT EXISTS ux_chauffeurs_code ON chauffeurs(code)",
        "ALTER TABLE vehicules ADD COLUMN IF NOT EXISTS niveau_carburant_pct NUMERIC(5,2) NOT NULL DEFAULT 100",
        "ALTER TABLE vehicules ADD COLUMN IF NOT EXISTS temperature_consigne_c NUMERIC(5,2)"
    };
    for (const char* sql : instructions) executerOuLever(db, sql);
}

} // namespace

// --- Clients ---
void PersistancePostgres::sauvegarderClients(Database& db, ClientRepository& repo) {
    Transaction transaction(db); preparerSchema(db);
    for (const Client* c : repo.tousLesElements()) {
        executerOuLever(db, "INSERT INTO clients (code, nom, telephone, adresse) VALUES ($1, $2, $3, $4) "
            "ON CONFLICT (code) DO UPDATE SET nom = EXCLUDED.nom, telephone = EXCLUDED.telephone, adresse = EXCLUDED.adresse",
            {c->getId(), c->getNom(), c->getTelephone(), c->getAdresse()});
    }
    transaction.valider();
}

ResumeChargement PersistancePostgres::chargerClients(Database& db, ClientRepository& repo) {
    Transaction transaction(db); preparerSchema(db);
    QueryResult lignes = interrogerOuLever(db, "SELECT code, nom, telephone, adresse FROM clients WHERE code IS NOT NULL ORDER BY id");
    transaction.valider();
    ResumeChargement resume;
    for (int i = 0; i < lignes.rowCount(); ++i) {
        const std::string code = lignes.value(i, 0);
        if (repo.trouverParId(code) != nullptr) { ++resume.ignores; continue; }
        repo.ajouter(std::make_unique<Client>(code, lignes.value(i, 1), lignes.value(i, 2), lignes.value(i, 3)));
        ++resume.ajoutes;
    }
    return resume;
}

// --- Chauffeurs ---
void PersistancePostgres::sauvegarderChauffeurs(Database& db, ChauffeurRepository& repo) {
    Transaction transaction(db); preparerSchema(db);
    for (const Chauffeur* c : repo.tousLesElements()) {
        executerOuLever(db, "INSERT INTO chauffeurs (code, nom, numero_permis, type_permis, experience_annees, statut) "
            "VALUES ($1, $2, $1, $3, $4, $5) ON CONFLICT (code) DO UPDATE SET nom = EXCLUDED.nom, type_permis = EXCLUDED.type_permis, experience_annees = EXCLUDED.experience_annees, statut = EXCLUDED.statut",
            {c->getId(), c->getNom(), codePermis(c->getTypePermis()), std::to_string(c->getExperienceAnnees()), codeStatut(c->getStatut())});
    }
    transaction.valider();
}

ResumeChargement PersistancePostgres::chargerChauffeurs(Database& db, ChauffeurRepository& repo) {
    Transaction transaction(db); preparerSchema(db);
    QueryResult lignes = interrogerOuLever(db, "SELECT code, nom, type_permis, experience_annees, statut FROM chauffeurs WHERE code IS NOT NULL ORDER BY id");
    transaction.valider();
    ResumeChargement resume;
    for (int i = 0; i < lignes.rowCount(); ++i) {
        const std::string code = lignes.value(i, 0);
        if (repo.trouverParId(code) != nullptr) { ++resume.ignores; continue; }
        auto chauffeur = std::make_unique<Chauffeur>(code, lignes.value(i, 1), permisDepuisCode(lignes.value(i, 2)), texteVersEntier(lignes.value(i, 3), 0));
        chauffeur->setStatut(statutChauffeurDepuisCode(lignes.value(i, 4)));
        repo.ajouter(std::move(chauffeur));
        ++resume.ajoutes;
    }
    return resume;
}

// --- Flotte ---
void PersistancePostgres::sauvegarderFlotte(Database& db, GestionnaireFlotte& flotte) {
    Transaction transaction(db); preparerSchema(db);
    for (Vehicule* v : flotte.tousLesVehicules()) {
        SqlParam temperature = std::nullopt;
        if (v->estFrigorifique()) temperature = nombreVersTexte(static_cast<const VehiculeFrigorifique&>(*v).getTemperatureConsigneC());
        executerOuLever(db, "INSERT INTO vehicules (immatriculation, marque, modele, type, capacite_kg, kilometrage, niveau_carburant_pct, temperature_consigne_c, statut) "
            "VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9) ON CONFLICT (immatriculation) DO UPDATE SET marque = EXCLUDED.marque, modele = EXCLUDED.modele, type = EXCLUDED.type, capacite_kg = EXCLUDED.capacite_kg, kilometrage = EXCLUDED.kilometrage, niveau_carburant_pct = EXCLUDED.niveau_carburant_pct, temperature_consigne_c = EXCLUDED.temperature_consigne_c, statut = EXCLUDED.statut",
            {v->getImmatriculation(), v->getMarque(), v->getModele(), v->getType(), nombreVersTexte(v->getCapaciteKg()), nombreVersTexte(v->getKilometrage()), nombreVersTexte(v->getNiveauCarburantPct()), temperature, codeStatut(v->getStatut())});
    }
    transaction.valider();
}

ResumeChargement PersistancePostgres::chargerFlotte(Database& db, GestionnaireFlotte& flotte) {
    Transaction transaction(db); preparerSchema(db);
    QueryResult lignes = interrogerOuLever(db, "SELECT immatriculation, marque, modele, type, capacite_kg, kilometrage, niveau_carburant_pct, temperature_consigne_c, statut FROM vehicules ORDER BY id");
    transaction.valider();
    ResumeChargement resume;
    for (int i = 0; i < lignes.rowCount(); ++i) {
        ParametresVehicule p;
        p.immatriculation = lignes.value(i, 0); p.marque = lignes.value(i, 1); p.modele = lignes.value(i, 2);
        p.capaciteKg = texteVersNombre(lignes.value(i, 4), 0.0); p.kilometrage = texteVersNombre(lignes.value(i, 5), 0.0);
        p.niveauCarburantPct = texteVersNombre(lignes.value(i, 6), 100.0);
        p.temperatureConsigneC = lignes.isNull(i, 7) ? 4.0 : texteVersNombre(lignes.value(i, 7), 4.0);
        if (flotte.trouverParImmatriculation(p.immatriculation) != nullptr) { ++resume.ignores; continue; }
        try {
            auto vehicule = VehiculeFactory::creer(lignes.value(i, 3), p);
            vehicule->setStatut(statutVehiculeDepuisCode(lignes.value(i, 8)));
            flotte.ajouterVehicule(std::move(vehicule));
            ++resume.ajoutes;
        } catch (const TypeInconnuException&) { ++resume.ignores; }
    }
    return resume;
}

// --- Tout en une fois ---
void PersistancePostgres::sauvegarderTout(Database& db, ClientRepository& clients, ChauffeurRepository& chauffeurs, GestionnaireFlotte& flotte) {
    sauvegarderClients(db, clients);
    sauvegarderChauffeurs(db, chauffeurs);
    sauvegarderFlotte(db, flotte);
}

ResumeChargement PersistancePostgres::chargerTout(Database& db, ClientRepository& clients, ChauffeurRepository& chauffeurs, GestionnaireFlotte& flotte) {
    ResumeChargement total;
    total += chargerClients(db, clients);
    total += chargerChauffeurs(db, chauffeurs);
    total += chargerFlotte(db, flotte);
    return total;
}

} // namespace sl