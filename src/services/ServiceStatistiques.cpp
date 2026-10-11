#include "services/ServiceStatistiques.hpp"
#include <iostream>
#include <iomanip>

namespace sl {

RapportStatistiques ServiceStatistiques::genererRapport(const std::vector<Livraison*>& livraisons,
                                                          GestionnaireFlotte& flotte) {
    RapportStatistiques rapport;
    rapport.livraisonsTotal = static_cast<int>(livraisons.size());

    for (Livraison* l : livraisons) {
        switch (l->getStatut()) {
            case StatutLivraison::PLANIFIEE: rapport.livraisonsPlanifiees++; break;
            case StatutLivraison::EN_COURS:  rapport.livraisonsEnCours++;   break;
            case StatutLivraison::LIVREE:    rapport.livraisonsTerminees++; break;
            case StatutLivraison::RETARDEE:  rapport.livraisonsRetardees++; break;
            case StatutLivraison::ANNULEE:   rapport.livraisonsAnnulees++;  break;
        }
        rapport.distanceTotaleKm  += l->getItineraire().distanceTotaleKm;
        rapport.dureeTotaleHeures += l->getItineraire().dureeTotaleHeures;
        rapport.coutTotalAr       += l->getItineraire().coutTotalAr;
    }

    auto vehicules = flotte.tousLesVehicules();
    rapport.vehiculesTotal = static_cast<int>(vehicules.size());
    for (Vehicule* v : vehicules) {
        if (v->getStatut() == StatutVehicule::DISPONIBLE) rapport.vehiculesDisponibles++;
        if (v->getStatut() == StatutVehicule::EN_MAINTENANCE) rapport.vehiculesEnMaintenance++;
    }

    return rapport;
}

void ServiceStatistiques::afficherRapport(const RapportStatistiques& r) {
    std::cout << "==============================\n";
    std::cout << "STATISTIQUES\n";
    std::cout << "==============================\n";
    std::cout << "Livraisons total       : " << r.livraisonsTotal << "\n";
    std::cout << "  Planifiees           : " << r.livraisonsPlanifiees << "\n";
    std::cout << "  En cours             : " << r.livraisonsEnCours << "\n";
    std::cout << "  Terminees            : " << r.livraisonsTerminees << "\n";
    std::cout << "  Retardees            : " << r.livraisonsRetardees << "\n";
    std::cout << "  Annulees             : " << r.livraisonsAnnulees << "\n";
    std::cout << "Vehicules total        : " << r.vehiculesTotal << "\n";
    std::cout << "  Disponibles          : " << r.vehiculesDisponibles << "\n";
    std::cout << "  En maintenance       : " << r.vehiculesEnMaintenance << "\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Distance parcourue     : " << r.distanceTotaleKm << " km\n";
    std::cout << "Duree cumulee          : " << r.dureeTotaleHeures << " h\n";
    std::cout << "Cout transport total   : " << r.coutTotalAr << " Ar\n";
    std::cout << "==============================\n";
}

} // namespace sl
