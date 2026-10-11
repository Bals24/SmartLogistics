#include "entities/Moto.hpp"

namespace sl {

std::string Moto::getType() const { return "MOTO"; }
TypePermis Moto::permisRequis() const { return TypePermis::MOTO; }

} // namespace sl
