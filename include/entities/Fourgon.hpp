#ifndef SMARTLOGISTICS_FOURGON_HPP
#define SMARTLOGISTICS_FOURGON_HPP

#include "entities/Vehicule.hpp"

namespace sl {

class Fourgon : public Vehicule {
public:
    using Vehicule::Vehicule;

    std::string getType() const override;
    TypePermis permisRequis() const override;
};

} // namespace sl

#endif // SMARTLOGISTICS_FOURGON_HPP
