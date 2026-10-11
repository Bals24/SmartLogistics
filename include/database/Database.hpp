#pragma once

#include <string>

#include <libpq-fe.h>

namespace SmartLogistics
{
    /**
     * Gestion centralisée de la connexion PostgreSQL.
     *
     * Cette classe sera utilisée plus tard par les repositories :
     *
     * GUI
     *  ↓
     * Services
     *  ↓
     * Repositories
     *  ↓
     * Database
     *  ↓
     * PostgreSQL
     */
    class Database
    {
    public:

        /**
         * Constructeur.
         */
        Database();

        /**
         * Destructeur.
         *
         * Ferme automatiquement la connexion PostgreSQL.
         */
        ~Database();

        /**
         * Établit la connexion à PostgreSQL.
         *
         * @return true si la connexion fonctionne.
         */
        bool connect();

        /**
         * Ferme la connexion.
         */
        void disconnect();

        /**
         * Vérifie si la connexion est active.
         */
        bool isConnected() const;

        /**
         * Retourne la connexion PostgreSQL.
         */
        PGconn* connection() const;

        /**
         * Retourne le dernier message d'erreur.
         */
        std::string lastError() const;

    private:

        PGconn* connection_;

        // Paramètres PostgreSQL
        std::string host_;
        std::string port_;
        std::string database_;
        std::string user_;
        std::string password_;
    };
}