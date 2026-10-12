#pragma once

#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <libpq-fe.h>

namespace SmartLogistics
{
    // Parametre SQL : std::nullopt = NULL.
    using SqlParam = std::optional<std::string>;

    /**
     * Resultat d'une requete (enveloppe RAII autour de PGresult :
     * PQclear est appele automatiquement).
     */
    class QueryResult
    {
    public:
        QueryResult() = default;
        explicit QueryResult(PGresult* result);
        ~QueryResult();

        QueryResult(const QueryResult&) = delete;
        QueryResult& operator=(const QueryResult&) = delete;
        QueryResult(QueryResult&& other) noexcept;
        QueryResult& operator=(QueryResult&& other) noexcept;

        /** true si la requete a reussi (commande ou SELECT). */
        bool ok() const;

        int rowCount() const;
        int columnCount() const;

        bool isNull(int row, int column) const;

        /** Valeur textuelle de la cellule ("" si NULL). */
        std::string value(int row, int column) const;

    private:
        PGresult* result_ = nullptr;
    };

    /**
     * Gestion centralisee de la connexion PostgreSQL.
     *
     * Parametres de connexion (valeurs par defaut entre parentheses) :
     *   SL_DB_HOST      (::1)
     *   SL_DB_PORT      (5432)
     *   SL_DB_NAME      (smart_logistics)
     *   SL_DB_USER      (postgres)
     *   SL_DB_PASSWORD  (aucun : libpq utilise alors PGPASSWORD / pgpass.conf)
     *
     * Le mot de passe ne doit JAMAIS etre ecrit dans le code source.
     */
    class Database
    {
    public:
        Database();
        ~Database();

        Database(const Database&) = delete;
        Database& operator=(const Database&) = delete;

        bool connect();
        void disconnect();
        bool isConnected() const;

        PGconn* connection() const;
        std::string lastError() const;

        /**
         * Execute une requete SQL. Les valeurs variables doivent passer
         * par params ($1, $2, ...) et jamais etre collees dans le texte
         * de la requete (protection contre l'injection SQL).
         */
        QueryResult query(const std::string& sql,
                          const std::vector<SqlParam>& params = {});

        /** Comme query(), mais renvoie simplement true/false. */
        bool execute(const std::string& sql,
                     const std::vector<SqlParam>& params = {});

    private:
        PGconn* connection_;

        std::string host_;
        std::string port_;
        std::string database_;
        std::string user_;
        std::string password_;
    };
}