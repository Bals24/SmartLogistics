#ifndef SMARTLOGISTICS_VEHICULEFRIGORIFIQUE_HPP
#define SMARTLOGISTICS_VEHICULEFRIGORIFIQUE_HPP

#include "entities/Vehicule.hpp"

namespace sl {

class VehiculeFrigorifique : public Vehicule {
public:
    VehiculeFrigorifique(std::string immatriculation,
                          std::string marque,
                          std::string modele,
                          double capaciteKg,
                          double temperatureConsigneC,
                          double kilometrage = 0.0,
                          double niveauCarburantPct = 100.0);

    std::string getType() const override;
    TypePermis permisRequis() const override;
    bool estFrigorifique() const override;
    void afficherInformations() const override;

    double getTemperatureConsigneC() const;
    void definirTemperatureConsigne(double tempC);

private:
    double temperatureConsigneC_;
};

} // namespace sl

#endif // SMARTLOGISTICS_VEHICULEFRIGORIFIQUE_HPP
