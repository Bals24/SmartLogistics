#include "factories/ColisFactory.hpp"
#include "entities/ColisStandard.hpp"
#include "entities/ColisFragile.hpp"
#include "entities/ColisExpress.hpp"
#include "entities/ColisRefrigere.hpp"
#include "utils/Exceptions.hpp"
#include <algorithm>

namespace sl {

namespace {
std::string versMajuscules(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    return s;
}
}

std::unique_ptr<Colis> ColisFactory::creer(const std::string& type, const ParametresColis& p) {
    std::string typeNormalise = versMajuscules(type);

    if (typeNormalise == "STANDARD") {
        return std::make_unique<ColisStandard>(p.id, p.description, p.poidsKg, p.volumeM3,
                                                 p.origine, p.destination, p.priorite);
    }
    if (typeNormalise == "FRAGILE") {
        std::string consigne = p.consigneManutention.value_or("Manipuler avec precaution");
        return std::make_unique<ColisFragile>(p.id, p.description, p.poidsKg, p.volumeM3,
                                                p.origine, p.destination, consigne, p.priorite);
    }
    if (typeNormalise == "EXPRESS") {
        int delai = p.delaiMaxHeures.value_or(24);
        return std::make_unique<ColisExpress>(p.id, p.description, p.poidsKg, p.volumeM3,
                                                p.origine, p.destination, delai);
    }
    if (typeNormalise == "REFRIGERE") {
        double tMin = p.temperatureMinC.value_or(2.0);
        double tMax = p.temperatureMaxC.value_or(8.0);
        return std::make_unique<ColisRefrigere>(p.id, p.description, p.poidsKg, p.volumeM3,
                                                   p.origine, p.destination, tMin, tMax, p.priorite);
    }
    throw TypeInconnuException(type);
}

} // namespace sl
