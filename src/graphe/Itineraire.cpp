#include "graphe/Itineraire.hpp"
#include <iostream>

namespace sl {

Itineraire::Itineraire() = default;

void Itineraire::afficherInformations() const {
    std::cout << "------------------------------\n";
    if (!trouve) {
        std::cout << "ITINERAIRE : aucun chemin trouve\n";
        std::cout << "------------------------------\n";
        return;
    }
    std::cout << "ITINERAIRE (strategie : " << strategieUtilisee << ")\n";
    for (std::size_t i = 0; i < villes.size(); ++i) {
        std::cout << villes[i];
        if (i + 1 < villes.size()) std::cout << " -> ";
    }
    std::cout << "\n";
    std::cout << "Distance : " << distanceTotaleKm << " km\n";
    std::cout << "Temps    : " << dureeTotaleHeures << " h\n";
    std::cout << "Cout     : " << coutTotalAr << " Ar\n";
    std::cout << "------------------------------\n";
}

} // namespace sl
