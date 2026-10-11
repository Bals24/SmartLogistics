#include "entities/Enums.hpp"

namespace sl {

std::string toString(StatutColis statut) {
    switch (statut) {
        case StatutColis::EN_ATTENTE:    return "EN_ATTENTE";
        case StatutColis::PREPARATION:   return "PREPARATION";
        case StatutColis::PRET:          return "PRET";
        case StatutColis::EN_TRANSIT:    return "EN_TRANSIT";
        case StatutColis::EN_LIVRAISON:  return "EN_LIVRAISON";
        case StatutColis::LIVRE:         return "LIVRE";
        case StatutColis::RETOURNE:      return "RETOURNE";
        case StatutColis::INCIDENT:      return "INCIDENT";
    }
    return "INCONNU";
}

std::string toString(Priorite priorite) {
    switch (priorite) {
        case Priorite::STANDARD: return "STANDARD";
        case Priorite::EXPRESS:  return "EXPRESS";
        case Priorite::URGENTE:  return "URGENTE";
    }
    return "INCONNU";
}

std::string toString(StatutCommande statut) {
    switch (statut) {
        case StatutCommande::CREEE:                return "CREEE";
        case StatutCommande::CONFIRMEE:             return "CONFIRMEE";
        case StatutCommande::PREPARATION:           return "PREPARATION";
        case StatutCommande::COLIS_PRET:            return "COLIS_PRET";
        case StatutCommande::AFFECTATION_VEHICULE:  return "AFFECTATION_VEHICULE";
        case StatutCommande::EXPEDITION:            return "EXPEDITION";
        case StatutCommande::EN_TRANSIT:            return "EN_TRANSIT";
        case StatutCommande::EN_LIVRAISON:          return "EN_LIVRAISON";
        case StatutCommande::LIVREE:                return "LIVREE";
        case StatutCommande::ANNULEE:                return "ANNULEE";
        case StatutCommande::RETOURNEE:             return "RETOURNEE";
        case StatutCommande::INCIDENT:              return "INCIDENT";
    }
    return "INCONNU";
}

std::string toString(StatutVehicule statut) {
    switch (statut) {
        case StatutVehicule::DISPONIBLE:      return "DISPONIBLE";
        case StatutVehicule::EN_MISSION:      return "EN_MISSION";
        case StatutVehicule::EN_MAINTENANCE:  return "EN_MAINTENANCE";
        case StatutVehicule::INDISPONIBLE:    return "INDISPONIBLE";
    }
    return "INCONNU";
}

std::string toString(StatutChauffeur statut) {
    switch (statut) {
        case StatutChauffeur::DISPONIBLE:    return "DISPONIBLE";
        case StatutChauffeur::EN_MISSION:    return "EN_MISSION";
        case StatutChauffeur::EN_CONGE:      return "EN_CONGE";
        case StatutChauffeur::INDISPONIBLE:  return "INDISPONIBLE";
    }
    return "INCONNU";
}

std::string toString(TypePermis type) {
    switch (type) {
        case TypePermis::B:    return "B";
        case TypePermis::C:    return "C (Poids lourd)";
        case TypePermis::C1:   return "C1 (Poids lourd leger)";
        case TypePermis::MOTO: return "MOTO";
    }
    return "INCONNU";
}

std::string toString(GraviteIncident gravite) {
    switch (gravite) {
        case GraviteIncident::FAIBLE:   return "FAIBLE";
        case GraviteIncident::MOYENNE:  return "MOYENNE";
        case GraviteIncident::ELEVEE:   return "ELEVEE";
        case GraviteIncident::CRITIQUE: return "CRITIQUE";
    }
    return "INCONNU";
}

std::string toString(StatutLivraison statut) {
    switch (statut) {
        case StatutLivraison::PLANIFIEE: return "PLANIFIEE";
        case StatutLivraison::EN_COURS:  return "EN_COURS";
        case StatutLivraison::LIVREE:    return "LIVREE";
        case StatutLivraison::RETARDEE:  return "RETARDEE";
        case StatutLivraison::ANNULEE:   return "ANNULEE";
    }
    return "INCONNU";
}

std::string toString(TypeMaintenance type) {
    switch (type) {
        case TypeMaintenance::VIDANGE:            return "VIDANGE";
        case TypeMaintenance::FREINS:             return "FREINS";
        case TypeMaintenance::PNEUS:              return "PNEUS";
        case TypeMaintenance::MOTEUR:             return "MOTEUR";
        case TypeMaintenance::REVISION_GENERALE:  return "REVISION_GENERALE";
    }
    return "INCONNU";
}

std::string toString(EtatRoute etat) {
    switch (etat) {
        case EtatRoute::LIBRE:    return "LIBRE";
        case EtatRoute::RALENTIE: return "RALENTIE";
        case EtatRoute::BLOQUEE:  return "BLOQUEE";
    }
    return "INCONNU";
}

} // namespace sl
