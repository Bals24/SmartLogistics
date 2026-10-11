#ifndef SMARTLOGISTICS_COLISREFRIGERE_HPP
#define SMARTLOGISTICS_COLISREFRIGERE_HPP

#include "entities/Colis.hpp"

namespace sl {

class ColisRefrigere : public Colis {
public:
    ColisRefrigere(std::string id,
                   std::string description,
                   double poidsKg,
                   double volumeM3,
                   std::string origine,
                   std::string destination,
                   double temperatureMinC,
                   double temperatureMaxC,
                   Priorite priorite = Priorite::STANDARD);

    std::string getType() const override;
    void afficherInformations() const override;
    bool necessiteVehiculeSpecial() const override;

    double getTemperatureMinC() const;
    double getTemperatureMaxC() const;

private:
    double temperatureMinC_;
    double temperatureMaxC_;
};

} // namespace sl

#endif // SMARTLOGISTICS_COLISREFRIGERE_HPP
