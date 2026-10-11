#include "graphe/Dijkstra.hpp"
#include <unordered_map>
#include <queue>
#include <limits>
#include <algorithm>

namespace sl {

namespace {
constexpr double INFINI = std::numeric_limits<double>::infinity();
}

Itineraire calculerPlusCourtChemin(const Graphe& graphe,
                                    const std::string& depart,
                                    const std::string& arrivee,
                                    const StrategieItineraire& strategie) {
    Itineraire resultat;
    resultat.strategieUtilisee = strategie.nom();

    if (!graphe.contientVille(depart) || !graphe.contientVille(arrivee)) {
        return resultat; // trouve reste false
    }

    // distancePonderee[ville] = meilleur poids cumulé connu depuis "depart"
    std::unordered_map<std::string, double> distancePonderee;
    // predecesseur[ville] = (ville precedente, route empruntee)
    std::unordered_map<std::string, std::pair<std::string, Route>> predecesseur;

    for (const auto& ville : graphe.getVilles()) {
        distancePonderee[ville] = INFINI;
    }
    distancePonderee[depart] = 0.0;

    using PaireDistanceVille = std::pair<double, std::string>;
    std::priority_queue<PaireDistanceVille, std::vector<PaireDistanceVille>,
                         std::greater<PaireDistanceVille>> filePriorite;
    filePriorite.push({0.0, depart});

    std::unordered_map<std::string, bool> visite;

    while (!filePriorite.empty()) {
        auto [distActuelle, villeActuelle] = filePriorite.top();
        filePriorite.pop();

        if (visite[villeActuelle]) continue;
        visite[villeActuelle] = true;

        if (villeActuelle == arrivee) break;

        for (const auto& route : graphe.getRoutesDepuis(villeActuelle)) {
            double poidsRoute = strategie.poids(route);
            if (poidsRoute == INFINI) continue; // route bloquée, on l'ignore

            std::string voisin = route.autreVille(villeActuelle);
            double nouvelleDistance = distActuelle + poidsRoute;

            if (nouvelleDistance < distancePonderee[voisin]) {
                distancePonderee[voisin] = nouvelleDistance;
                predecesseur.insert_or_assign(voisin, std::make_pair(villeActuelle, route));
                filePriorite.push({nouvelleDistance, voisin});
            }
        }
    }

    if (distancePonderee.find(arrivee) == distancePonderee.end() ||
        distancePonderee[arrivee] == INFINI) {
        return resultat; // aucun chemin trouvé
    }

    // Reconstruction du chemin en remontant les prédécesseurs.
    std::vector<std::string> cheminInverse;
    std::vector<Route> routesUtilisees;
    std::string courante = arrivee;
    cheminInverse.push_back(courante);

    while (courante != depart) {
        auto it = predecesseur.find(courante);
        if (it == predecesseur.end()) {
            return resultat; // ne devrait pas arriver, sécurité
        }
        routesUtilisees.push_back(it->second.second);
        courante = it->second.first;
        cheminInverse.push_back(courante);
    }

    std::reverse(cheminInverse.begin(), cheminInverse.end());
    std::reverse(routesUtilisees.begin(), routesUtilisees.end());

    resultat.trouve = true;
    resultat.villes = std::move(cheminInverse);
    for (const auto& route : routesUtilisees) {
        resultat.distanceTotaleKm += route.getDistanceKm();
        resultat.dureeTotaleHeures += route.getTempsHeures();
        resultat.coutTotalAr += route.getCoutAr();
    }

    return resultat;
}

} // namespace sl
