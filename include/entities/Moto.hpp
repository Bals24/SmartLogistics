#ifndef SMARTLOGISTICS_MOTO_HPP
#define SMARTLOGISTICS_MOTO_HPP

#include "entities/Vehicule.hpp"

namespace sl {

class Moto : public Vehicule {
public:
    using Vehicule::Vehicule;

    std::string getType() const override;
    TypePermis permisRequis() const override;
};

} // namespace sl

#endif // SMARTLOGISTICS_MOTO_HPP
