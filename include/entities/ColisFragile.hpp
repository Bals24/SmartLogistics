#ifndef SMARTLOGISTICS_COLISFRAGILE_HPP
#define SMARTLOGISTICS_COLISFRAGILE_HPP

#include "entities/Colis.hpp"

namespace sl {

class ColisFragile : public Colis {
public:
    ColisFragile(std::string id,
                 std::string description,
                 double poidsKg,
                 double volumeM3,
                 std::string origine,
                 std::string destination,
                 std::string consigneManutention,
                 Priorite priorite = Priorite::STANDARD);

    std::string getType() const override;
    void afficherInformations() const override;

    const std::string& getConsigneManutention() const;

private:
    std::string consigneManutention_; // ex: "Ne pas empiler", "Haut / Bas"
};

} // namespace sl

#endif // SMARTLOGISTICS_COLISFRAGILE_HPP
