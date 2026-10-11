#include "utils/IdGenerator.hpp"
#include <cstdio>

namespace sl {

std::atomic<int> IdGenerator::compteur_{0};

std::string IdGenerator::suivant(const std::string& prefixe) {
    int n = ++compteur_;
    char buffer[16];
    std::snprintf(buffer, sizeof(buffer), "%04d", n);
    return prefixe + "-" + buffer;
}

} // namespace sl
