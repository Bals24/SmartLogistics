#ifndef SMARTLOGISTICS_SURVEILLANCEFLOTTE_HPP
#define SMARTLOGISTICS_SURVEILLANCEFLOTTE_HPP

#include <thread>
#include <atomic>
#include <chrono>
#include "services/GestionnaireFlotte.hpp"
#include "services/ServiceMaintenance.hpp"

namespace sl {

// Niveau 6 - Concurrence : surveille la flotte dans un thread separe pendant
// que le thread principal continue de faire tourner le menu console. La
// tache est reelle (pas un thread "pour faire joli") : elle detecte et
// marque en arriere-plan les vehicules qui viennent de depasser leur seuil
// de maintenance, pendant que l'utilisateur peut continuer a naviguer dans
// le menu. Toutes les donnees partagees (GestionnaireFlotte, ServiceMaintenance)
// sont protegees par leur propre mutex interne.
class SurveillanceFlotte {
public:
    SurveillanceFlotte(GestionnaireFlotte& flotte,
                        ServiceMaintenance& maintenance,
                        std::chrono::milliseconds intervalle);
    ~SurveillanceFlotte();

    SurveillanceFlotte(const SurveillanceFlotte&) = delete;
    SurveillanceFlotte& operator=(const SurveillanceFlotte&) = delete;

    void demarrer();
    void arreter();
    bool estActive() const;

private:
    void boucle();

    GestionnaireFlotte& flotte_;
    ServiceMaintenance& maintenance_;
    std::chrono::milliseconds intervalle_;
    std::atomic<bool> enCours_;
    std::thread thread_;
};

} // namespace sl

#endif // SMARTLOGISTICS_SURVEILLANCEFLOTTE_HPP
