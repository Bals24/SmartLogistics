#include "services/SurveillanceFlotte.hpp"
#include <iostream>

namespace sl {

SurveillanceFlotte::SurveillanceFlotte(GestionnaireFlotte& flotte,
                                        ServiceMaintenance& maintenance,
                                        std::chrono::milliseconds intervalle)
    : flotte_(flotte), maintenance_(maintenance), intervalle_(intervalle), enCours_(false) {}

SurveillanceFlotte::~SurveillanceFlotte() {
    arreter();
}

void SurveillanceFlotte::demarrer() {
    if (enCours_) return;
    enCours_ = true;
    thread_ = std::thread(&SurveillanceFlotte::boucle, this);
}

void SurveillanceFlotte::arreter() {
    enCours_ = false;
    if (thread_.joinable()) thread_.join();
}

bool SurveillanceFlotte::estActive() const { return enCours_; }

void SurveillanceFlotte::boucle() {
    while (enCours_) {
        auto vehicules = maintenance_.detecterEtMarquer(flotte_);
        for (Vehicule* v : vehicules) {
            std::cout << "\n[SURVEILLANCE FLOTTE] (thread arriere-plan) Vehicule "
                       << v->getImmatriculation()
                       << " marque EN_MAINTENANCE (kilometrage : "
                       << v->getKilometrage() << " km)\n";
        }
        std::this_thread::sleep_for(intervalle_);
    }
}

} // namespace sl
