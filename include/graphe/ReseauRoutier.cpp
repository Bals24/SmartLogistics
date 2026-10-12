#include "graphe/ReseauRoutier.hpp"

namespace sl {

void construireReseauMalgache(Graphe& graphe) {
    // Distances (km), durees (heures) et couts (Ar) APPROXIMATIFS,
    // pour la demonstration.

    // --- Hauts plateaux et cote est ---
    graphe.ajouterRoute(Route("Antananarivo", "Antsirabe", 170.0, 3.0, 60000.0));
    graphe.ajouterRoute(Route("Antananarivo", "Moramanga", 110.0, 2.0, 45000.0));
    graphe.ajouterRoute(Route("Antsirabe", "Ambositra", 90.0, 2.0, 35000.0));
    graphe.ajouterRoute(Route("Ambositra", "Moramanga", 260.0, 5.5, 100000.0));
    graphe.ajouterRoute(Route("Moramanga", "Toamasina", 210.0, 4.0, 90000.0));
    // Route directe plus chere mais plus rapide entre Antsirabe et Toamasina,
    // partiellement ralentie pour illustrer l'impact sur la strategie "plus rapide".
    graphe.ajouterRoute(Route("Antsirabe", "Toamasina", 340.0, 6.0, 150000.0, EtatRoute::RALENTIE));

    // --- Sud (RN7) ---
    graphe.ajouterRoute(Route("Ambositra", "Fianarantsoa", 130.0, 2.5, 50000.0));
    graphe.ajouterRoute(Route("Fianarantsoa", "Toliara", 700.0, 12.0, 250000.0));

    // --- Nord-ouest (RN4) et extreme nord ---
    graphe.ajouterRoute(Route("Antananarivo", "Maevatanana", 280.0, 5.0, 100000.0));
    graphe.ajouterRoute(Route("Maevatanana", "Mahajanga", 290.0, 5.0, 100000.0));
    graphe.ajouterRoute(Route("Mahajanga", "Antsohihy", 420.0, 8.0, 150000.0));
    graphe.ajouterRoute(Route("Antsohihy", "Antsiranana", 700.0, 13.0, 250000.0));
}

} // namespace sl