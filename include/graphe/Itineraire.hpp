#ifndef SMARTLOGISTICS_ITINERAIRE_HPP
#define SMARTLOGISTICS_ITINERAIRE_HPP

#include <string>
#include <vector>

namespace sl {

// Résultat structuré d'un calcul de plus court chemin : la liste ordonnée
// des villes traversées, et les totaux réels (pas seulement le poids
// optimisé par la stratégie utilisée) le long de ce chemin.
class Itineraire {
public:
    Itineraire();

    bool trouve = false;
    std::vector<std::string> villes;
    double distanceTotaleKm = 0.0;
    double dureeTotaleHeures = 0.0;
    double coutTotalAr = 0.0;
    std::string strategieUtilisee; // nom de la stratégie ayant guidé le calcul

    void afficherInformations() const;
};

} // namespace sl

#endif // SMARTLOGISTICS_ITINERAIRE_HPP
