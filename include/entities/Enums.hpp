#ifndef SMARTLOGISTICS_ENUMS_HPP
#define SMARTLOGISTICS_ENUMS_HPP

#include <string>

namespace sl {

// Statut générique d'un colis
enum class StatutColis {
    EN_ATTENTE,
    PREPARATION,
    PRET,
    EN_TRANSIT,
    EN_LIVRAISON,
    LIVRE,
    RETOURNE,
    INCIDENT
};

// Niveau de priorité d'un colis / d'une commande
enum class Priorite {
    STANDARD,
    EXPRESS,
    URGENTE
};

// Statut d'une commande (base du futur State Pattern, Niveau 5)
enum class StatutCommande {
    CREEE,
    CONFIRMEE,
    PREPARATION,
    COLIS_PRET,
    AFFECTATION_VEHICULE,
    EXPEDITION,
    EN_TRANSIT,
    EN_LIVRAISON,
    LIVREE,
    ANNULEE,
    RETOURNEE,
    INCIDENT
};

// Statut opérationnel d'un véhicule
enum class StatutVehicule {
    DISPONIBLE,
    EN_MISSION,
    EN_MAINTENANCE,
    INDISPONIBLE
};

// Statut d'un chauffeur
enum class StatutChauffeur {
    DISPONIBLE,
    EN_MISSION,
    EN_CONGE,
    INDISPONIBLE
};

// Type de permis de conduire
enum class TypePermis {
    B,          // véhicule léger
    C,          // poids lourd
    C1,         // poids lourd léger
    MOTO
};

// Gravité d'un incident logistique
enum class GraviteIncident {
    FAIBLE,
    MOYENNE,
    ELEVEE,
    CRITIQUE
};

// Statut d'une livraison
enum class StatutLivraison {
    PLANIFIEE,
    EN_COURS,
    LIVREE,
    RETARDEE,
    ANNULEE
};

// Type d'opération de maintenance
enum class TypeMaintenance {
    VIDANGE,
    FREINS,
    PNEUS,
    MOTEUR,
    REVISION_GENERALE
};

// État de praticabilité d'une route du graphe
enum class EtatRoute {
    LIBRE,
    RALENTIE,
    BLOQUEE
};

// --- Fonctions utilitaires de conversion enum -> texte (affichage) ---

std::string toString(StatutColis statut);
std::string toString(Priorite priorite);
std::string toString(StatutCommande statut);
std::string toString(StatutVehicule statut);
std::string toString(StatutChauffeur statut);
std::string toString(TypePermis type);
std::string toString(GraviteIncident gravite);
std::string toString(StatutLivraison statut);
std::string toString(TypeMaintenance type);
std::string toString(EtatRoute etat);

} // namespace sl

#endif // SMARTLOGISTICS_ENUMS_HPP
