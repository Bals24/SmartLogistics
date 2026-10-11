#ifndef SMARTLOGISTICS_COLISEXPRESS_HPP
#define SMARTLOGISTICS_COLISEXPRESS_HPP

#include "entities/Colis.hpp"

namespace sl {

class ColisExpress : public Colis {
public:
    ColisExpress(std::string id,
                 std::string description,
                 double poidsKg,
                 double volumeM3,
                 std::string origine,
                 std::string destination,
                 int delaiMaxHeures);

    std::string getType() const override;
    void afficherInformations() const override;

    int getDelaiMaxHeures() const;

private:
    int delaiMaxHeures_;
};

} // namespace sl

#endif // SMARTLOGISTICS_COLISEXPRESS_HPP
