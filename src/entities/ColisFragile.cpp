#include "entities/ColisFragile.hpp"
#include <iostream>

namespace sl {

ColisFragile::ColisFragile(std::string id,
                            std::string description,
                            double poidsKg,
                            double volumeM3,
                            std::string origine,
                            std::string destination,
                            std::string consigneManutention,
                            Priorite priorite)
    : Colis(std::move(id), std::move(description), poidsKg, volumeM3,
            std::move(origine), std::move(destination), priorite),
      consigneManutention_(std::move(consigneManutention)) {}

std::string ColisFragile::getType() const { return "FRAGILE"; }

const std::string& ColisFragile::getConsigneManutention() const {
    return consigneManutention_;
}

void ColisFragile::afficherInformations() const {
    afficherEntete();
    std::cout << "Consigne    : " << consigneManutention_ << "\n";
    std::cout << "==============================\n";
}

} // namespace sl
