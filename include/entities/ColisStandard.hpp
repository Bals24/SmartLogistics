#ifndef SMARTLOGISTICS_COLISSTANDARD_HPP
#define SMARTLOGISTICS_COLISSTANDARD_HPP

#include "entities/Colis.hpp"

namespace sl {

class ColisStandard : public Colis {
public:
    using Colis::Colis; // hérite des constructeurs de Colis

    std::string getType() const override;
};

} // namespace sl

#endif // SMARTLOGISTICS_COLISSTANDARD_HPP
