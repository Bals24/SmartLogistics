#include "entities/Fourgon.hpp"

namespace sl {

std::string Fourgon::getType() const { return "FOURGON"; }
TypePermis Fourgon::permisRequis() const { return TypePermis::B; }

} // namespace sl
