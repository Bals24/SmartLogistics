#pragma once

#include <memory>

#include <QString>
#include <QStringList>

class QTableWidget;

/*
 * Pont entre les tables de l'interface (QTableWidget) et PostgreSQL.
 *
 * Les pages sont synchronisees table par table. La colonne 0 de chaque
 * table (CLI-001, VEH-001, #CMD-001, ...) sert de cle : elle est stockee
 * dans la colonne "code" (ou "reference" pour commandes et colis) de la
 * table SQL correspondante.
 *
 * L'ordre de l'enumeration est l'ordre des dependances (cles etrangeres) :
 * on enregistre toujours les clients avant les commandes, etc.
 */
enum class TableId
{
    Clients = 0,
    Vehicules,
    Chauffeurs,
    Entrepots,
    Itineraires,
    Commandes,
    Colis,
    Livraisons,
    Incidents,
    Maintenance
};

constexpr int NOMBRE_TABLES = 10;

struct ResultatSync
{
    bool ok = true;                   // false si la connexion ou une requete globale echoue
    int lignes = 0;                   // lignes chargees / enregistrees
    bool suppressionRefusee = false;  // une suppression a ete refusee par la base
    QStringList erreurs;              // erreurs (une ligne en erreur n'arrete pas les autres)
};

class GuiDatabase
{
public:
    GuiDatabase();
    ~GuiDatabase();

    GuiDatabase(const GuiDatabase&) = delete;
    GuiDatabase& operator=(const GuiDatabase&) = delete;

    /*
     * Remplace le contenu de la table par celui de la base.
     * Si la base ne contient rien pour cette table (lignes == 0), la table
     * de l'interface est videe quand viderSiVide vaut true, sinon laissee
     * telle quelle.
     */
    ResultatSync charger(TableId id, QTableWidget* table, bool viderSiVide = false);

    /*
     * Enregistre toutes les lignes de la table (INSERT ou UPDATE) et
     * supprime de la base les lignes que l'utilisateur a supprimees.
     */
    ResultatSync sauvegarder(TableId id, QTableWidget* table);

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};