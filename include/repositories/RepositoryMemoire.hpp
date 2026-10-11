#ifndef SMARTLOGISTICS_REPOSITORYMEMOIRE_HPP
#define SMARTLOGISTICS_REPOSITORYMEMOIRE_HPP

#include "repositories/IRepository.hpp"
#include <unordered_map>
#include <memory>
#include <algorithm>

namespace sl {

// Implémentation en mémoire du Repository Pattern (Niveau 4 pourra ajouter
// une implémentation SQLite/JSON derrière la même interface IRepository,
// sans changer le code des services qui l'utilisent).
template <typename T, typename IdType = std::string>
class RepositoryMemoire : public IRepository<T, IdType> {
public:
    void ajouter(std::unique_ptr<T> element) override {
        IdType id = element->getId();
        elements_[id] = std::move(element);
        ordre_.push_back(id);
    }

    T* trouverParId(const IdType& id) override {
        auto it = elements_.find(id);
        return (it != elements_.end()) ? it->second.get() : nullptr;
    }

    const T* trouverParId(const IdType& id) const override {
        auto it = elements_.find(id);
        return (it != elements_.end()) ? it->second.get() : nullptr;
    }

    std::vector<T*> tousLesElements() override {
        std::vector<T*> resultat;
        resultat.reserve(elements_.size());
        for (const auto& id : ordre_) {
            auto it = elements_.find(id);
            if (it != elements_.end()) resultat.push_back(it->second.get());
        }
        return resultat;
    }

    std::vector<const T*> tousLesElements() const override {
        std::vector<const T*> resultat;
        resultat.reserve(elements_.size());
        for (const auto& id : ordre_) {
            auto it = elements_.find(id);
            if (it != elements_.end()) resultat.push_back(it->second.get());
        }
        return resultat;
    }

    bool supprimer(const IdType& id) override {
        auto it = elements_.find(id);
        if (it == elements_.end()) return false;
        elements_.erase(it);
        ordre_.erase(std::remove(ordre_.begin(), ordre_.end(), id), ordre_.end());
        return true;
    }

    std::size_t taille() const override { return elements_.size(); }

private:
    std::unordered_map<IdType, std::unique_ptr<T>> elements_;
    std::vector<IdType> ordre_; // préserve l'ordre d'insertion pour l'affichage
};

} // namespace sl

#endif // SMARTLOGISTICS_REPOSITORYMEMOIRE_HPP
