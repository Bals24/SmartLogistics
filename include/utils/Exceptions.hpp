#ifndef SMARTLOGISTICS_EXCEPTIONS_HPP
#define SMARTLOGISTICS_EXCEPTIONS_HPP

#include <stdexcept>
#include <string>

namespace sl {

// Exception de base pour toutes les erreurs du domaine métier.
class SmartLogisticsException : public std::runtime_error {
public:
    explicit SmartLogisticsException(const std::string& message)
        : std::runtime_error(message) {}
};

// Levée par les factories quand un type demandé est inconnu.
class TypeInconnuException : public SmartLogisticsException {
public:
    explicit TypeInconnuException(const std::string& type)
        : SmartLogisticsException("Type inconnu : " + type) {}
};

// Levée quand un élément (id) n'existe pas dans un repository.
class ElementIntrouvableException : public SmartLogisticsException {
public:
    explicit ElementIntrouvableException(const std::string& id)
        : SmartLogisticsException("Element introuvable avec l'id : " + id) {}
};

// Levée quand une transition d'état est invalide (State Pattern).
class TransitionInvalideException : public SmartLogisticsException {
public:
    TransitionInvalideException(const std::string& etatActuel, const std::string& action)
        : SmartLogisticsException("Transition invalide : impossible de faire '" +
                                   action + "' depuis l'etat '" + etatActuel + "'") {}
};

// Levée quand aucun chemin n'existe entre deux villes du graphe.
class CheminIntrouvableException : public SmartLogisticsException {
public:
    CheminIntrouvableException(const std::string& depart, const std::string& arrivee)
        : SmartLogisticsException("Aucun chemin trouve entre " + depart + " et " + arrivee) {}
};

// Levée quand une ressource (véhicule, chauffeur) ne peut pas être affectée.
class AffectationImpossibleException : public SmartLogisticsException {
public:
    explicit AffectationImpossibleException(const std::string& raison)
        : SmartLogisticsException("Affectation impossible : " + raison) {}
};

// Levée lors d'un échec de lecture/écriture de fichier de persistance.
class PersistanceException : public SmartLogisticsException {
public:
    explicit PersistanceException(const std::string& chemin)
        : SmartLogisticsException("Erreur de persistance sur le fichier : " + chemin) {}
};

} // namespace sl

#endif // SMARTLOGISTICS_EXCEPTIONS_HPP
