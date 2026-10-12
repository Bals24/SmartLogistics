#include "graphe/ReseauRoutier.hpp"
#include "graphe/Route.hpp"

namespace sl {

void construireReseauMalgache(Graphe& graphe)
{
    // Fonction utilitaire pour créer et ajouter une route bidirectionnelle
    auto ajouterRoute = [&graphe](const std::string& a, const std::string& b, 
                                  double dist, double temps, double cout, 
                                  EtatRoute etat = EtatRoute::LIBRE) 
    {
        graphe.ajouterRoute(Route(a, b, dist, temps, cout, etat), true);
    };

    // Antananarivo <-> Toamasina (via Moramanga)
    ajouterRoute("Antananarivo", "Moramanga", 110.0, 2.0, 55000.0);
    ajouterRoute("Moramanga", "Toamasina", 210.0, 4.0, 105000.0);

    // Antananarivo <-> Antsirabe
    ajouterRoute("Antananarivo", "Antsirabe", 170.0, 3.0, 85000.0);

    // Antsirabe <-> Fianarantsoa (via Ambositra)
    ajouterRoute("Antsirabe", "Ambositra", 120.0, 2.5, 60000.0);
    ajouterRoute("Ambositra", "Fianarantsoa", 100.0, 2.0, 50000.0);

    // Antananarivo <-> Mahajanga (via Maevatanana)
    ajouterRoute("Antananarivo", "Maevatanana", 380.0, 7.0, 190000.0);
    ajouterRoute("Maevatanana", "Mahajanga", 190.0, 3.0, 95000.0);

    // Fianarantsoa <-> Toliara (via Ranohira)
    ajouterRoute("Fianarantsoa", "Ranohira", 180.0, 4.0, 90000.0);
    ajouterRoute("Ranohira", "Toliara", 170.0, 3.0, 85000.0);

    // Mahajanga <-> Antsiranana
    ajouterRoute("Mahajanga", "Antsiranana", 1100.0, 18.0, 550000.0);
}

} // namespace sl