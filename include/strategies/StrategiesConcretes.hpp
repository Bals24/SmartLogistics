#ifndef SMARTLOGISTICS_STRATEGIESCONCRETES_HPP
#define SMARTLOGISTICS_STRATEGIESCONCRETES_HPP

#include "strategies/StrategieItineraire.hpp"

namespace sl {

class StrategiePlusCourtChemin : public StrategieItineraire {
public:
    double poids(const Route& route) const override;
    std::string nom() const override;
};

class StrategiePlusRapide : public StrategieItineraire {
public:
    double poids(const Route& route) const override;
    std::string nom() const override;
};

class StrategieMoinsCher : public StrategieItineraire {
public:
    double poids(const Route& route) const override;
    std::string nom() const override;
};

} // namespace sl

#endif // SMARTLOGISTICS_STRATEGIESCONCRETES_HPP
