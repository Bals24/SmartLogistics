#ifndef SMARTLOGISTICS_CAMION_HPP
#define SMARTLOGISTICS_CAMION_HPP

#include "entities/Vehicule.hpp"

namespace sl {

class Camion : public Vehicule {
public:
    using Vehicule::Vehicule;

    std::string getType() const override;
    TypePermis permisRequis() const override;
};

} // namespace sl

#endif // SMARTLOGISTICS_CAMION_HPP
