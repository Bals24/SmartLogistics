#include "GuiDatabase.hpp"
#include <QDate>
#include <QRegularExpression>
#include <QSet>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <cmath>
#include <cstdlib>
#include <string>
#include <vector>

#ifdef SL_HAS_POSTGRES
#include "database/Database.hpp"
#endif

#ifndef SL_HAS_POSTGRES

struct GuiDatabase::Impl {};
GuiDatabase::GuiDatabase() : impl_(std::make_unique<Impl>()) {}
GuiDatabase::~GuiDatabase() = default;

static ResultatSync horsLigne()
{
    ResultatSync r;
    r.ok = false;
    r.erreurs << "PostgreSQL n'est pas disponible dans cette compilation.";
    return r;
}

ResultatSync GuiDatabase::charger(TableId, QTableWidget*, bool) { return horsLigne(); }
ResultatSync GuiDatabase::sauvegarder(TableId, QTableWidget*) { return horsLigne(); }

#else

using SmartLogistics::Database;
using SmartLogistics::QueryResult;
using SmartLogistics::SqlParam;

namespace
{
    enum class Genre
    {
        Texte, Code, Km, Ar, Kg, Colis, KgEntier, Pourcent, Annees, Duree,
        Date, Diese, Ref, Compteur, Cle, NomItineraire
    };

    struct Col
    {
        int gui = 0;
        const char* db = "";
        Genre genre = Genre::Texte;
        const char* exprChargement = nullptr;
        bool charge = true;
        bool sauve = true;
        bool majConflit = true;
        bool requis = false;
        const char* refTable = nullptr;
        const char* refCle = nullptr;
    };

    struct Spec
    {
        const char* table;
        const char* cle;
        const char* prefixe;
        const char* cleNaturelle;
        std::vector<Col> cols;
    };

    Col cleCol(const char* db = "code", Genre g = Genre::Texte)
    {
        Col c; c.gui = 0; c.db = db; c.genre = g; c.majConflit = false; c.requis = true;
        return c;
    }

    Col col(int gui, const char* db, Genre g = Genre::Texte, bool requis = false)
    {
        Col c; c.gui = gui; c.db = db; c.genre = g; c.requis = requis;
        return c;
    }

    Col copie(int gui, const char* db, Genre g, bool requis, bool majConflit = true)
    {
        Col c; c.gui = gui; c.db = db; c.genre = g; c.charge = false; c.requis = requis; c.majConflit = majConflit;
        return c;
    }

    Col calculee(int gui, const char* db, const char* expr)
    {
        Col c; c.gui = gui; c.db = db; c.genre = Genre::Compteur; c.exprChargement = expr; c.sauve = false; c.majConflit = false;
        return c;
    }

    Col ref(int gui, const char* db, const char* table, const char* cleRef, bool requis = false)
    {
        Col c; c.gui = gui; c.db = db; c.genre = Genre::Ref; c.requis = requis; c.refTable = table; c.refCle = cleRef;
        return c;
    }

    const Spec& spec(TableId id)
    {
        static const Spec clients{
            "clients", "code", "CLI", nullptr,
            { cleCol(), col(1, "prenom", Genre::Texte, true), col(2, "nom", Genre::Texte, true),
              col(3, "telephone"), col(4, "email"), col(5, "ville"),
              calculee(6, "commandes", "(SELECT COUNT(*) FROM commandes WHERE commandes.client_id = clients.id)") }};

        static const Spec vehicules{
            "vehicules", "code", "VEH", "immatriculation",
            { cleCol(), col(1, "immatriculation", Genre::Texte, true), col(2, "marque"), col(3, "modele"),
              col(4, "type", Genre::Code), col(5, "statut", Genre::Code), col(6, "kilometrage", Genre::Km),
              col(7, "capacite_kg", Genre::KgEntier), col(8, "niveau_carburant_pct", Genre::Pourcent) }};

        static const Spec chauffeurs{
            "chauffeurs", "code", "CHA", nullptr,
            { cleCol(), col(1, "nom", Genre::Texte, true), col(2, "telephone"), copie(0, "numero_permis", Genre::Cle, false, false),
              col(3, "type_permis"), col(4, "vehicule_affecte"), col(5, "statut", Genre::Code), col(6, "experience_annees", Genre::Annees) }};

        static const Spec entrepots{
            "entrepots", "code", "ENT", nullptr,
            { cleCol(), col(1, "nom", Genre::Texte, true), col(2, "ville"), col(3, "capacite", Genre::Colis),
              col(4, "stock_actuel", Genre::Colis), col(5, "statut", Genre::Code) }};

        static const Spec itineraires{
            "itineraires", "code", "ITI", nullptr,
            { cleCol(), copie(0, "nom", Genre::NomItineraire, false), col(1, "origine", Genre::Texte, true),
              col(2, "destination", Genre::Texte, true), col(3, "distance_km", Genre::Km), col(4, "duree_minutes", Genre::Duree),
              col(5, "statut", Genre::Code), col(6, "trajet") }};

        static const Spec commandes{
            "commandes", "reference", "CMD", nullptr,
            { cleCol("reference", Genre::Diese),
              ref(1, "client_id", "clients", "nom", true),
              col(2, "ville_livraison"), copie(2, "adresse_livraison", Genre::Texte, true),
              col(3, "statut", Genre::Code), col(4, "date_commande", Genre::Date, true), col(5, "montant", Genre::Ar) }};

        static const Spec colis{
            "colis", "reference", "COL", nullptr,
            { cleCol("reference"), ref(1, "commande_id", "commandes", "reference", true), col(2, "type_colis"),
              col(3, "poids_kg", Genre::Kg), col(4, "statut", Genre::Code), col(5, "destination"), col(6, "date_colis", Genre::Date) }};

        static const Spec livraisons{
            "livraisons", "code", "LIV", nullptr,
            { cleCol(), ref(1, "commande_id", "commandes", "reference", true),
              calculee(2, "client", "(SELECT cl.nom FROM commandes cm JOIN clients cl ON cl.id = cm.client_id WHERE cm.id = livraisons.commande_id)"),
              ref(3, "chauffeur_id", "chauffeurs", "nom"),
              ref(4, "vehicule_id", "vehicules", "immatriculation"),
              col(5, "adresse_arrivee"), col(6, "statut", Genre::Code, true) }};

        static const Spec incidents{
            "incidents", "code", "INC", nullptr,
            { cleCol(), col(1, "date_incident", Genre::Date, true), col(2, "type", Genre::Texte, true),
              ref(3, "vehicule_id", "vehicules", "immatriculation"),
              ref(4, "chauffeur_id", "chauffeurs", "nom"),
              col(5, "description", Genre::Texte, true), col(6, "statut", Genre::Code, true) }};

        static const Spec maintenance{
            "maintenance", "code", "MAI", nullptr,
            { cleCol(), ref(1, "vehicule_id", "vehicules", "immatriculation", true),
              col(2, "type", Genre::Texte, true), col(3, "date_planifiee", Genre::Date), col(4, "kilometrage", Genre::Km),
              col(5, "cout", Genre::Ar), col(6, "statut", Genre::Code, true) }};

        switch (id)
        {
            case TableId::Clients:     return clients;
            case TableId::Vehicules:   return vehicules;
            case TableId::Chauffeurs:  return chauffeurs;
            case TableId::Entrepots:   return entrepots;
            case TableId::Itineraires: return itineraires;
            case TableId::Commandes:   return commandes;
            case TableId::Colis:       return colis;
            case TableId::Livraisons:  return livraisons;
            case TableId::Incidents:   return incidents;
            case TableId::Maintenance: return maintenance;
        }
        return clients;
    }

    QString texteCellule(const QTableWidget* table, int ligne, int colonne)
    {
        if (colonne < 0 || colonne >= table->columnCount()) return {};
        const QTableWidgetItem* item = table->item(ligne, colonne);
        return item ? item->text().trimmed() : QString();
    }

    QString groupeParTrois(long long valeur)
    {
        QString chiffres = QString::number(std::llabs(valeur));
        QString resultat;
        int compteur = 0;
        for (int i = chiffres.size() - 1; i >= 0; --i) {
            if (compteur > 0 && compteur % 3 == 0) resultat.prepend(' ');
            resultat.prepend(chiffres[i]);
            ++compteur;
        }
        return valeur < 0 ? "-" + resultat : resultat;
    }

    QString codeDepuisAffichage(QString texte) { return texte.trimmed().toUpper().replace(' ', '_'); }
    QString affichageDepuisCode(QString code) {
        QString texte = code.replace('_', ' ').toLower();
        if (!texte.isEmpty()) texte[0] = texte[0].toUpper();
        return texte;
    }
    QString chiffresSeulement(const QString& texte) {
        QString resultat = texte; resultat.remove(QRegularExpression("[^0-9]"));
        return resultat.isEmpty() ? QString("0") : resultat;
    }
    QString nombreDepuisAffichage(QString texte) {
        texte.remove(QRegularExpression("[^0-9,.]")); texte.replace(',', '.');
        return texte.isEmpty() ? QString("0") : texte;
    }
    QString sansDiese(QString texte) {
        texte = texte.trimmed(); if (texte.startsWith('#')) texte.remove(0, 1); return texte;
    }
    int minutesDepuisAffichage(const QString& texte) {
        static const QRegularExpression heures("^\\s*(\\d+)\\s*h\\s*(\\d+)?");
        const QRegularExpressionMatch m = heures.match(texte);
        if (m.hasMatch()) return m.captured(1).toInt() * 60 + m.captured(2).toInt();
        return chiffresSeulement(texte).toInt();
    }
    QString affichageDepuisMinutes(int minutes) {
        if (minutes <= 0) return "0h";
        if (minutes < 60) return QString("%1 min").arg(minutes);
        const int h = minutes / 60; const int m = minutes % 60;
        return m == 0 ? QString("%1h").arg(h) : QString("%1h %2").arg(h).arg(m, 2, 10, QChar('0'));
    }

    SqlParam versBase(const Col& col, const QTableWidget* table, int ligne, const QString& cle)
    {
        QString texte = texteCellule(table, ligne, col.gui);
        switch (col.genre) {
            case Genre::Cle: return cle.toStdString();
            case Genre::NomItineraire: return (texteCellule(table, ligne, 1) + " - " + texteCellule(table, ligne, 2)).toStdString();
            case Genre::Code: if (texte.isEmpty()) return std::nullopt; return codeDepuisAffichage(texte).toStdString();
            case Genre::Km: case Genre::Ar: case Genre::Kg: case Genre::KgEntier: case Genre::Pourcent:
                return nombreDepuisAffichage(texte).toStdString();
            case Genre::Colis: case Genre::Annees: return chiffresSeulement(texte).toStdString();
            case Genre::Duree: return std::to_string(minutesDepuisAffichage(texte));
            case Genre::Date: {
                const QDate date = QDate::fromString(texte, "dd/MM/yyyy");
                if (date.isValid()) return date.toString(Qt::ISODate).toStdString();
                if (col.requis) return QDate::currentDate().toString(Qt::ISODate).toStdString();
                return std::nullopt;
            }
            case Genre::Diese: return sansDiese(texte).toStdString();
            case Genre::Ref: texte = sansDiese(texte); if (texte.isEmpty()) return std::nullopt; return texte.toStdString();
            case Genre::Compteur: case Genre::Texte: break;
        }
        if (texte.isEmpty() && !col.requis) return std::nullopt;
        return texte.toStdString();
    }

    QString versAffichage(const Col& col, const std::string& valeur)
    {
        const QString brut = QString::fromStdString(valeur);
        switch (col.genre) {
            case Genre::Code: return affichageDepuisCode(brut);
            case Genre::Km: return groupeParTrois(std::llround(std::atof(valeur.c_str()))) + " km";
            case Genre::Ar: return groupeParTrois(std::llround(std::atof(valeur.c_str()))) + " Ar";
            case Genre::Kg: return QString::number(std::atof(valeur.c_str()), 'f', 1) + " kg";
            case Genre::Colis: return QString::number(std::atoi(valeur.c_str())) + " colis";
            case Genre::KgEntier: return groupeParTrois(std::llround(std::atof(valeur.c_str()))) + " kg";
            case Genre::Pourcent: return QString::number(std::llround(std::atof(valeur.c_str()))) + " %";
            case Genre::Annees: return QString::number(std::atoi(valeur.c_str())) + " ans";
            case Genre::Duree: return affichageDepuisMinutes(std::atoi(valeur.c_str()));
            case Genre::Date: {
                if (valeur.empty()) return {};
                const QDate date = QDate::fromString(brut.left(10), Qt::ISODate);
                return date.isValid() ? date.toString("dd/MM/yyyy") : brut;
            }
            case Genre::Diese: return "#" + brut;
            case Genre::Ref: return brut.isEmpty() ? QString("Aucun") : brut;
            default: return brut;
        }
    }

    std::string premiereLigne(const std::string& message) {
        const auto fin = message.find('\n');
        return fin == std::string::npos ? message : message.substr(0, fin);
    }

    QString messageLisible(const std::string& brut) {
        const QString ligne = QString::fromStdString(premiereLigne(brut));
        if (ligne.contains("not-null constraint")) return "valeur obligatoire manquante ou reference introuvable";
        if (ligne.contains("duplicate key")) return "valeur deja utilisee par une autre ligne (doublon)";
        if (ligne.contains("foreign key")) return "element utilise par d'autres donnees";
        if (ligne.contains("check constraint")) return "valeur refusee par la base";
        return ligne;
    }

    const char* const instructionsSchema[] = {
        "ALTER TABLE clients     ADD COLUMN IF NOT EXISTS code VARCHAR(30)",
        "ALTER TABLE clients     ADD COLUMN IF NOT EXISTS prenom VARCHAR(100) NOT NULL DEFAULT ''",
        "CREATE UNIQUE INDEX IF NOT EXISTS ux_clients_code ON clients(code)",
        "ALTER TABLE vehicules   ADD COLUMN IF NOT EXISTS code VARCHAR(30)",
        "CREATE UNIQUE INDEX IF NOT EXISTS ux_vehicules_code ON vehicules(code)",
        "ALTER TABLE chauffeurs  ADD COLUMN IF NOT EXISTS code VARCHAR(30)",
        "ALTER TABLE chauffeurs  ADD COLUMN IF NOT EXISTS vehicule_affecte VARCHAR(30)",
        "ALTER TABLE chauffeurs  ADD COLUMN IF NOT EXISTS experience_annees INTEGER NOT NULL DEFAULT 0",
        "ALTER TABLE vehicules   ADD COLUMN IF NOT EXISTS niveau_carburant_pct NUMERIC(5,2) NOT NULL DEFAULT 100",
        "CREATE UNIQUE INDEX IF NOT EXISTS ux_chauffeurs_code ON chauffeurs(code)",
        "ALTER TABLE entrepots   ADD COLUMN IF NOT EXISTS code VARCHAR(30)",
        "CREATE UNIQUE INDEX IF NOT EXISTS ux_entrepots_code ON entrepots(code)",
        "ALTER TABLE itineraires ADD COLUMN IF NOT EXISTS code VARCHAR(30)",
        "ALTER TABLE itineraires ADD COLUMN IF NOT EXISTS trajet TEXT",
        "CREATE UNIQUE INDEX IF NOT EXISTS ux_itineraires_code ON itineraires(code)",
        "ALTER TABLE colis       ADD COLUMN IF NOT EXISTS type_colis VARCHAR(30)",
        "ALTER TABLE colis       ADD COLUMN IF NOT EXISTS destination VARCHAR(100)",
        "ALTER TABLE colis       ADD COLUMN IF NOT EXISTS date_colis DATE",
        "ALTER TABLE livraisons  ADD COLUMN IF NOT EXISTS code VARCHAR(30)",
        "CREATE UNIQUE INDEX IF NOT EXISTS ux_livraisons_code ON livraisons(code)",
        "ALTER TABLE incidents   ADD COLUMN IF NOT EXISTS code VARCHAR(30)",
        "CREATE UNIQUE INDEX IF NOT EXISTS ux_incidents_code ON incidents(code)",
        "ALTER TABLE maintenance ADD COLUMN IF NOT EXISTS code VARCHAR(30)",
        "ALTER TABLE maintenance ADD COLUMN IF NOT EXISTS kilometrage NUMERIC(12,2) NOT NULL DEFAULT 0",
        "CREATE UNIQUE INDEX IF NOT EXISTS ux_maintenance_code ON maintenance(code)"
    };

    std::string expressionLecture(const Spec& s, const Col& c) {
        if (c.exprChargement != nullptr) return c.exprChargement;
        if (c.genre == Genre::Ref) return std::string("(SELECT ") + c.refCle + " FROM " + c.refTable + " WHERE id = " + s.table + "." + c.db + ")";
        return c.db;
    }

    std::string expressionEcriture(const Col& c, std::size_t numeroParametre) {
        const std::string parametre = "$" + std::to_string(numeroParametre);
        if (c.genre == Genre::Ref) return std::string("(SELECT id FROM ") + c.refTable + " WHERE " + c.refCle + " = " + parametre + " ORDER BY id LIMIT 1)";
        return parametre;
    }
}

struct GuiDatabase::Impl
{
    Database db;
    bool schemaPret = false;

    QString assurerConnexion()
    {
        if (!db.isConnected()) {
            schemaPret = false;
            if (!db.connect()) return QString::fromStdString(premiereLigne(db.lastError()));
        }
        if (!schemaPret) {
            if (!db.execute("BEGIN")) return QString::fromStdString(premiereLigne(db.lastError()));
            for (const char* sql : instructionsSchema) {
                if (!db.execute(sql)) {
                    const QString erreur = QString::fromStdString(premiereLigne(db.lastError()));
                    db.execute("ROLLBACK");
                    return erreur + " (les tables existent-elles ? Executez d'abord bdd.txt)";
                }
            }
            if (!db.execute("COMMIT")) return QString::fromStdString(premiereLigne(db.lastError()));
            schemaPret = true;
        }
        return {};
    }
};

GuiDatabase::GuiDatabase() : impl_(std::make_unique<Impl>()) {}
GuiDatabase::~GuiDatabase() = default;

ResultatSync GuiDatabase::charger(TableId id, QTableWidget* table, bool viderSiVide)
{
    ResultatSync resultat;
    const Spec& s = spec(id);
    Database& db = impl_->db;
    const QString erreurConnexion = impl_->assurerConnexion();
    if (!erreurConnexion.isEmpty()) { resultat.ok = false; resultat.erreurs << erreurConnexion; return resultat; }

    const std::string table_ = s.table;
    const std::string cle_ = s.cle;
    const std::string prefixe_ = s.prefixe;

    if (cle_ == "code") {
        db.execute("UPDATE " + table_ + " SET code = '" + prefixe_ + "-' || lpad((m.maxn + r.rn)::text, 3, '0') "
            "FROM (SELECT id, row_number() OVER (ORDER BY id) AS rn FROM " + table_ + " WHERE code IS NULL) r, "
            "     (SELECT COALESCE(MAX(substring(code from '^" + prefixe_ + "-([0-9]+)$')::int), 0) AS maxn FROM " + table_ + ") m "
            "WHERE " + table_ + ".id = r.id");
    }

    std::vector<const Col*> colonnes;
    std::string select = "SELECT ";
    for (const Col& c : s.cols) {
        if (!c.charge) continue;
        if (!colonnes.empty()) select += ", ";
        select += expressionLecture(s, c);
        colonnes.push_back(&c);
    }
    select += " FROM " + table_ + " WHERE " + cle_ + " IS NOT NULL ORDER BY id";

    QueryResult lignes = db.query(select);
    if (!lignes.ok()) { resultat.ok = false; resultat.erreurs << messageLisible(db.lastError()); return resultat; }

    const int total = lignes.rowCount();
    if (total == 0) { if (viderSiVide) { table->clearSelection(); table->setRowCount(0); } return resultat; }

    table->clearSelection();
    table->setRowCount(0);
    table->setRowCount(total);

    for (int r = 0; r < total; ++r) {
        for (int i = 0; i < static_cast<int>(colonnes.size()); ++i) {
            const Col& c = *colonnes[i];
            if (c.gui >= table->columnCount()) continue;
            table->setItem(r, c.gui, new QTableWidgetItem(versAffichage(c, lignes.value(r, i))));
        }
    }
    resultat.lignes = total;
    return resultat;
}

ResultatSync GuiDatabase::sauvegarder(TableId id, QTableWidget* table)
{
    ResultatSync resultat;
    const Spec& s = spec(id);
    Database& db = impl_->db;
    const QString erreurConnexion = impl_->assurerConnexion();
    if (!erreurConnexion.isEmpty()) { resultat.ok = false; resultat.erreurs << erreurConnexion; return resultat; }

    const std::string table_ = s.table;
    const std::string cle_ = s.cle;

    std::vector<const Col*> colonnes;
    std::string noms, valeurs, miseAJour;
    for (const Col& c : s.cols) {
        if (!c.sauve) continue;
        colonnes.push_back(&c);
        if (!noms.empty()) { noms += ", "; valeurs += ", "; }
        noms += c.db;
        valeurs += expressionEcriture(c, colonnes.size());
        if (c.majConflit) {
            if (!miseAJour.empty()) miseAJour += ", ";
            miseAJour += std::string(c.db) + " = EXCLUDED." + c.db;
        }
    }

    const std::string insertion = "INSERT INTO " + table_ + " (" + noms + ") VALUES (" + valeurs + ") ON CONFLICT (" + cle_ + ") DO UPDATE SET " + miseAJour;

    if (!db.execute("BEGIN")) { resultat.ok = false; resultat.erreurs << QString::fromStdString(premiereLigne(db.lastError())); return resultat; }

    QueryResult existants = db.query("SELECT " + cle_ + " FROM " + table_ + " WHERE " + cle_ + " IS NOT NULL");
    if (!existants.ok()) { resultat.ok = false; resultat.erreurs << messageLisible(db.lastError()); db.execute("ROLLBACK"); return resultat; }

    QSet<QString> clesInterface;

    for (int r = 0; r < table->rowCount(); ++r) {
        const QString affichee = texteCellule(table, r, 0);
        if (affichee.isEmpty()) continue;

        const SqlParam cleParam = versBase(s.cols.front(), table, r, QString());
        const QString cle = QString::fromStdString(cleParam.value_or(std::string()));

        if (clesInterface.contains(cle)) { resultat.erreurs << QString("%1 : identifiant en double, ligne ignoree").arg(affichee); continue; }
        clesInterface.insert(cle);

        bool manquant = false;
        std::vector<SqlParam> params;
        for (const Col* c : colonnes) {
            SqlParam valeur = versBase(*c, table, r, cle);
            if (c->requis && (!valeur.has_value() || valeur->empty())) {
                resultat.erreurs << QString("%1 : valeur obligatoire manquante (%2)").arg(affichee, c->db);
                manquant = true;
                break;
            }
            params.push_back(std::move(valeur));
        }
        if (manquant) continue;

        db.execute("SAVEPOINT sl_ligne");
        
        // [LOGIQUE MÉTIER] Extraction de l'immatriculation si le format est "Marque Modele (IMMAT)"
        if (s.cleNaturelle != nullptr) {
            SqlParam cleNaturelle;
            for (std::size_t i = 0; i < colonnes.size(); ++i) {
                if (std::string(colonnes[i]->db) == s.cleNaturelle) cleNaturelle = params[i];
            }
            QString val = QString::fromStdString(cleNaturelle.value_or(""));
            QRegularExpression re("\\(([^)]+)\\)");
            QRegularExpressionMatch match = re.match(val);
            QString immat = match.hasMatch() ? match.captured(1) : val;

            db.execute("UPDATE " + table_ + " SET " + cle_ + " = $1 WHERE " + cle_ +
                       " IS NULL AND " + s.cleNaturelle + " = $2",
                       {cle.toStdString(), immat.toStdString()});
        }

        if (db.execute(insertion, params)) { db.execute("RELEASE SAVEPOINT sl_ligne"); ++resultat.lignes; }
        else { resultat.erreurs << QString("%1 : %2").arg(affichee, messageLisible(db.lastError())); db.execute("ROLLBACK TO SAVEPOINT sl_ligne"); db.execute("RELEASE SAVEPOINT sl_ligne"); }
    }

    for (int i = 0; i < existants.rowCount(); ++i) {
        const QString cle = QString::fromStdString(existants.value(i, 0));
        if (clesInterface.contains(cle)) continue;

        db.execute("SAVEPOINT sl_ligne");
        if (db.execute("DELETE FROM " + table_ + " WHERE " + cle_ + " = $1", {cle.toStdString()})) { db.execute("RELEASE SAVEPOINT sl_ligne"); }
        else { resultat.erreurs << QString("%1 : suppression impossible (%2)").arg(cle, messageLisible(db.lastError())); resultat.suppressionRefusee = true; db.execute("ROLLBACK TO SAVEPOINT sl_ligne"); db.execute("RELEASE SAVEPOINT sl_ligne"); }
    }

    if (!db.execute("COMMIT")) { resultat.ok = false; resultat.erreurs << QString::fromStdString(premiereLigne(db.lastError())); db.execute("ROLLBACK"); }
    return resultat;
}

#endif // SL_HAS_POSTGRES