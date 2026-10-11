#ifndef SMARTLOGISTICS_SERVICESTATISTIQUES_HPP
#define SMARTLOGISTICS_SERVICESTATISTIQUES_HPP

#include <vector>
#include "entities/Livraison.hpp"
#include "services/GestionnaireFlotte.hpp"

namespace sl {

struct RapportStatistiques {
    int livraisonsTotal = 0;
    int livraisonsPlanifiees = 0;
    int livraisonsEnCours = 0;
    int livraisonsTerminees = 0;
    int livraisonsRetardees = 0;
    int livraisonsAnnulees = 0;

    int vehiculesTotal = 0;
    int vehiculesDisponibles = 0;
    int vehiculesEnMaintenance = 0;

    double distanceTotaleKm = 0.0;
    double dureeTotaleHeures = 0.0;
    double coutTotalAr = 0.0;
};

// Agrege les donnees de livraisons + flotte en un rapport exploitable
// (equivalent du "tableau de bord" decrit dans la section 18 du cahier
// des charges).
class ServiceStatistiques {
public:
    static RapportStatistiques genererRapport(const std::vector<Livraison*>& livraisons,
                                               GestionnaireFlotte& flotte);
    static void afficherRapport(const RapportStatistiques& rapport);
};

} // namespace sl

#endif // SMARTLOGISTICS_SERVICESTATISTIQUES_HPP
