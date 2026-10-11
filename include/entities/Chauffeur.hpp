#ifndef SMARTLOGISTICS_CHAUFFEUR_HPP
#define SMARTLOGISTICS_CHAUFFEUR_HPP

#include <string>
#include "entities/Enums.hpp"

namespace sl {

class Chauffeur {
public:
    Chauffeur(std::string id,
              std::string nom,
              TypePermis typePermis,
              int experienceAnnees);

    const std::string& getId() const;
    const std::string& getNom() const;
    TypePermis getTypePermis() const;
    int getExperienceAnnees() const;
    StatutChauffeur getStatut() const;

    void setStatut(StatutChauffeur statut);

    // Règle métier : un chauffeur ne peut conduire un véhicule que si son
    // permis correspond au permis requis par le véhicule (cf. Vehicule::permisRequis).
    // Simplification Niveau 1 : le permis C autorise aussi le C1.
    bool peutConduire(TypePermis permisRequisPourVehicule) const;

    void afficherInformations() const;

private:
    std::string id_;
    std::string nom_;
    TypePermis typePermis_;
    int experienceAnnees_;
    StatutChauffeur statut_;
};

} // namespace sl

#endif // SMARTLOGISTICS_CHAUFFEUR_HPP
