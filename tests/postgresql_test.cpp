#include "database/Database.hpp"

#include <iostream>

int main()
{
    std::cout << "========================================\n";
    std::cout << "   SMART LOGISTICS - PostgreSQL Test\n";
    std::cout << "========================================\n\n";

    SmartLogistics::Database database;

    std::cout << "Connexion a PostgreSQL...\n";

    if (!database.connect())
    {
        std::cerr << "\nECHEC DE CONNEXION\n";
        std::cerr << database.lastError() << '\n';

        return 1;
    }

    std::cout << "\nCONNEXION PostgreSQL REUSSIE !\n";

    std::cout << "La base smart_logistics est accessible.\n";

    database.disconnect();

    std::cout << "\nConnexion fermee proprement.\n";

    return 0;
}