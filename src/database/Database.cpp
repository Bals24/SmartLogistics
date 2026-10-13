#include "database/Database.hpp"

#include <cstdlib>
#include <algorithm>

namespace SmartLogistics
{
    namespace
    {
        std::string variableOuDefaut(const char* nom, const std::string& defaut)
        {
            const char* valeur = std::getenv(nom);
            return (valeur != nullptr && *valeur != '\0') ? std::string(valeur) : defaut;
        }
    }

    // ============================================================
    // QueryResult
    // ============================================================

    QueryResult::QueryResult(PGresult* result)
        : result_(result)
    {
    }

    QueryResult::~QueryResult()
    {
        if (result_ != nullptr)
        {
            PQclear(result_);
        }
    }

    QueryResult::QueryResult(QueryResult&& other) noexcept
        : result_(std::exchange(other.result_, nullptr))
    {
    }

    QueryResult& QueryResult::operator=(QueryResult&& other) noexcept
    {
        if (this != &other)
        {
            if (result_ != nullptr)
            {
                PQclear(result_);
            }
            result_ = std::exchange(other.result_, nullptr);
        }
        return *this;
    }

    bool QueryResult::ok() const
    {
        if (result_ == nullptr)
        {
            return false;
        }

        const ExecStatusType status = PQresultStatus(result_);
        return status == PGRES_COMMAND_OK || status == PGRES_TUPLES_OK;
    }

    int QueryResult::rowCount() const
    {
        return result_ ? PQntuples(result_) : 0;
    }

    int QueryResult::columnCount() const
    {
        return result_ ? PQnfields(result_) : 0;
    }

    bool QueryResult::isNull(int row, int column) const
    {
        return result_ == nullptr || PQgetisnull(result_, row, column) == 1;
    }

    std::string QueryResult::value(int row, int column) const
    {
        if (isNull(row, column))
        {
            return {};
        }
        return std::string(PQgetvalue(result_, row, column));
    }

    // ============================================================
    // Database
    // ============================================================

    Database::Database()
        : connection_(nullptr),
          host_(variableOuDefaut("SL_DB_HOST", "127.0.0.1")),
          port_(variableOuDefaut("SL_DB_PORT", "5432")),
          database_(variableOuDefaut("SL_DB_NAME", "smart_logistics")),
          user_(variableOuDefaut("SL_DB_USER", "postgres")),
          password_("1234")
    {
    }

    Database::~Database()
    {
        disconnect();
    }

    bool Database::connect()
    {
        disconnect();

        // PQsetdbLogin évite les problèmes de caractères spéciaux dans le mot de passe.
        connection_ = PQsetdbLogin(
            host_.c_str(),
            port_.c_str(),
            nullptr,
            nullptr,
            database_.c_str(),
            user_.c_str(),
            password_.empty() ? nullptr : password_.c_str()
        );

        if (connection_ == nullptr)
        {
            return false;
        }

        if (PQstatus(connection_) != CONNECTION_OK)
        {
            return false;
        }

        // Les textes du programme sont en UTF-8 (accents des villes, etc.).
        PQsetClientEncoding(connection_, "UTF8");

        // Masque les NOTICE du serveur (ex : "la colonne existe déjà, ignorée")
        PQsetNoticeProcessor(connection_,
                             [](void*, const char*) {},
                             nullptr);

        return true;
    }

    void Database::disconnect()
    {
        if (connection_ != nullptr)
        {
            PQfinish(connection_);
            connection_ = nullptr;
        }
    }

    bool Database::isConnected() const
    {
        return connection_ != nullptr &&
               PQstatus(connection_) == CONNECTION_OK;
    }

    PGconn* Database::connection() const
    {
        return connection_;
    }

    std::string Database::lastError() const
    {
        if (connection_ == nullptr)
        {
            return "Aucune connexion PostgreSQL.";
        }

        const char* error = PQerrorMessage(connection_);

        if (error == nullptr || *error == '\0')
        {
            return "Erreur PostgreSQL inconnue.";
        }

        std::string message(error);
        while (!message.empty() &&
               (message.back() == '\n' || message.back() == '\r'))
        {
            message.pop_back();
        }
        return message;
    }

    QueryResult Database::query(const std::string& sql,
                                const std::vector<SqlParam>& params)
    {
        if (!isConnected())
        {
            return QueryResult();
        }

        std::vector<const char*> valeurs;
        valeurs.reserve(params.size());
        for (const SqlParam& param : params)
        {
            valeurs.push_back(param.has_value() ? param->c_str() : nullptr);
        }

        PGresult* brut = PQexecParams(
            connection_,
            sql.c_str(),
            static_cast<int>(params.size()),
            nullptr,                                   // types déduits par le serveur
            valeurs.empty() ? nullptr : valeurs.data(),
            nullptr,                                   // longueurs (texte)
            nullptr,                                   // formats (texte)
            0                                          // résultat en texte
        );

        return QueryResult(brut);
    }

    bool Database::execute(const std::string& sql,
                           const std::vector<SqlParam>& params)
    {
        return query(sql, params).ok();
    }
}