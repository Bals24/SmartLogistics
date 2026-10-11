#ifndef SMARTLOGISTICS_IDGENERATOR_HPP
#define SMARTLOGISTICS_IDGENERATOR_HPP

#include <string>
#include <atomic>

namespace sl {

// Génère des identifiants uniques "PREFIXE-0001", "PREFIXE-0002", ...
// Le compteur est un std::atomic : utilisable en toute sécurité depuis
// plusieurs threads (cf. Niveau 6 - Concurrence).
class IdGenerator {
public:
    static std::string suivant(const std::string& prefixe);

private:
    static std::atomic<int> compteur_;
};

} // namespace sl

#endif // SMARTLOGISTICS_IDGENERATOR_HPP
