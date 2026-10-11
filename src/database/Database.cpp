#include "database/Database.hpp"

#include <iostream>

namespace SmartLogistics
{
    // ============================================================
    // Constructeur
    // ============================================================

    Database::Database()
        : connection_(nullptr),

          // Configuration PostgreSQL
          host_("::1"),
          port_("5432"),
          database_("smart_logistics"),
          user_("postgres"),

          // Remplacer par le vrai mot de passe PostgreSQL
          password_("+++++++")
    {
    }


    // ============================================================
    // Destructeur
    // ============================================================

    Database::~Database()
    {
        disconnect();
    }


    // ============================================================
    // Connexion PostgreSQL
    // ============================================================

    bool Database::connect()
    {
        // Fermer une éventuelle connexion existante
        disconnect();

        // ========================================================
        // Connexion avec paramètres séparés
        // ========================================================
        //
        // PQsetdbLogin() évite les problèmes de caractères spéciaux
        // dans le mot de passe qui peuvent apparaître lors de la
        // construction manuelle d'une chaîne "host=... password=..."
        //
        // ========================================================

        connection_ = PQsetdbLogin(
            host_.c_str(),
            port_.c_str(),
            nullptr,              // options
            nullptr,              // tty
            database_.c_str(),
            user_.c_str(),
            password_.c_str()
        );

        // Vérifier que PostgreSQL a créé la connexion
        if (connection_ == nullptr)
        {
            return false;
        }

        // ========================================================
        // Vérification de l'état de la connexion
        // ========================================================

        if (PQstatus(connection_) != CONNECTION_OK)
        {
            return false;
        }

        return true;
    }


    // ============================================================
    // Déconnexion
    // ============================================================

    void Database::disconnect()
    {
        if (connection_ != nullptr)
        {
            PQfinish(connection_);
            connection_ = nullptr;
        }
    }


    // ============================================================
    // Vérification de la connexion
    // ============================================================

    bool Database::isConnected() const
    {
        return connection_ != nullptr &&
               PQstatus(connection_) == CONNECTION_OK;
    }


    // ============================================================
    // Accès à la connexion PostgreSQL
    // ============================================================

    PGconn* Database::connection() const
    {
        return connection_;
    }


    // ============================================================
    // Récupération de la dernière erreur
    // ============================================================

    std::string Database::lastError() const
    {
        if (connection_ == nullptr)
        {
            return "Aucune connexion PostgreSQL.";
        }

        const char* error = PQerrorMessage(connection_);

        if (error == nullptr)
        {
            return "Erreur PostgreSQL inconnue.";
        }

        return std::string(error);
    }
}