#ifndef SMARTLOGISTICS_RESEAUROUTIER_HPP
#define SMARTLOGISTICS_RESEAUROUTIER_HPP

#include "graphe/Graphe.hpp"

namespace sl {

// Remplit le graphe avec le reseau routier de demonstration (villes
// malgaches principales). Utilise par la console ET par l'interface
// graphique pour que les deux calculent les memes itineraires.
void construireReseauMalgache(Graphe& graphe);

} // namespace sl

#endif // SMARTLOGISTICS_RESEAUROUTIER_HPP