#ifndef SMARTLOGISTICS_IREPOSITORY_HPP
#define SMARTLOGISTICS_IREPOSITORY_HPP

#include <memory>
#include <string>
#include <vector>

namespace sl {

// Interface générique du Repository Pattern : sépare les objets métier de
// leur mécanisme de stockage. T doit exposer une méthode getId() renvoyant
// un identifiant convertible en IdType (std::string par défaut).
template <typename T, typename IdType = std::string>
class IRepository {
public:
    virtual ~IRepository() = default;

    virtual void ajouter(std::unique_ptr<T> element) = 0;
    virtual T* trouverParId(const IdType& id) = 0;
    virtual const T* trouverParId(const IdType& id) const = 0;
    virtual std::vector<T*> tousLesElements() = 0;
    virtual std::vector<const T*> tousLesElements() const = 0;
    virtual bool supprimer(const IdType& id) = 0;
    virtual std::size_t taille() const = 0;
};

} // namespace sl

#endif // SMARTLOGISTICS_IREPOSITORY_HPP
