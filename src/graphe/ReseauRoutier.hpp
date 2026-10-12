#ifndef SMARTLOGISTICS_RESEAURoutier_HPP
#define SMARTLOGISTICS_RESEAURoutier_HPP

#include "graphe/Graphe.hpp"

namespace sl {

// Construit le réseau routier malgache avec les villes principales
// et les routes entre elles.
void construireReseauMalgache(Graphe& graphe);

} // namespace sl

#endif // SMARTLOGISTICS_RESEAURoutier_HPP