#include "entities/Camion.hpp"

namespace sl {

std::string Camion::getType() const { return "CAMION"; }
TypePermis Camion::permisRequis() const { return TypePermis::C; }

} // namespace sl
