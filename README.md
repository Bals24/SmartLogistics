# SmartLogistics — Gestion intelligente de la logistique et de la flotte

SmartLogistics est une application conçue pour simplifier la gestion des activités logistiques d'une entreprise de transport. Elle regroupe les principales opérations dans un même système afin de faciliter le suivi des commandes, l'organisation des livraisons et la gestion des ressources.

Le projet dispose d'une application en console et d'une interface graphique de bureau. Il intègre également une architecture de gestion des données et un support de connexion à PostgreSQL.

## Fonctionnalités

### Gestion des opérations
- **Clients :** gestion des informations des clients.
- **Commandes :** création et suivi du cycle de vie des commandes.
- **Colis :** prise en charge de différents types de colis, notamment standards, fragiles, réfrigérés et express.
- **Entrepôts :** organisation des espaces de stockage.
- **Véhicules :** gestion des véhicules et de leurs caractéristiques.
- **Chauffeurs :** gestion des chauffeurs et des ressources humaines liées aux livraisons.
- **Livraisons :** suivi des opérations de livraison et des ressources affectées.
- **Itinéraires :** recherche de chemins et optimisation des déplacements.
- **Incidents :** gestion des incidents liés aux opérations.
- **Maintenance :** surveillance de la flotte et détection des besoins de maintenance.
- **Statistiques :** consultation d'indicateurs pour suivre l'activité logistique.

### Automatisation et optimisation
- Affectation des véhicules et des chauffeurs aux livraisons.
- Calcul d'itinéraires à partir d'un graphe de villes.
- Comparaison des itinéraires selon différents critères : distance, durée et coût.
- Suivi des changements d'état des commandes.
- Notifications lors des changements de statut.
- Sauvegarde et chargement de certaines données au format JSON.
- Surveillance de la flotte en arrière-plan.

### Interface et stockage
- Application en console pour accéder aux opérations du système.
- Interface graphique de bureau réalisée avec Qt.
- Architecture de dépôt permettant de séparer la logique métier du stockage des données.
- Support de connexion à une base de données PostgreSQL.
- Tests automatisés pour vérifier une partie du fonctionnement du système.

## Technologies utilisées

- **C++ :** logique métier, gestion des ressources et algorithmes.
- **Qt Widgets :** interface graphique.
- **CMake :** configuration et compilation du projet.
- **PostgreSQL / libpq :** accès à la base de données relationnelle.
- **JSON :** sauvegarde et chargement des données.
- **Git et GitHub :** gestion du code source et du suivi des modifications.

## Architecture du projet

```text
SmartLogistics/
├── CMakeLists.txt
├── README.md
├── bdd.txt
├── main.cpp
├── gui/
│   ├── main_gui.cpp
│   ├── MainWindow.cpp
│   ├── MainWindow.hpp
│   ├── GuiDatabase.cpp
│   ├── GuiDatabase.hpp
│   ├── FormulaireDialog.cpp
│   ├── FormulaireDialog.hpp
│   └── Style.hpp
├── include/
│   ├── database/
│   ├── entities/
│   ├── factories/
│   ├── graphe/
│   ├── observateurs/
│   ├── persistence/
│   ├── repositories/
│   ├── services/
│   ├── strategies/
│   └── ...
├── src/
│   ├── database/
│   ├── entities/
│   ├── factories/
│   ├── graphe/
│   ├── observateurs/
│   ├── persistence/
│   ├── services/
│   └── ...
├── tests/
├── data/
└── third_party/
    └── postgresql/
```

**Rôle des principaux dossiers :**

- `gui/` : interface graphique et composants de l'application de bureau.
- `include/` : déclarations des classes et interfaces du projet.
- `src/` : implémentation des entités, services et mécanismes métier.
- `tests/` : programmes de test.
- `data/` : données utilisées pour la sauvegarde et le chargement.
- `third_party/` : fichiers nécessaires à l'intégration de certaines dépendances.
- `bdd.txt` : document de référence du schéma de la base de données.

## Installation et compilation

### Prérequis

Pour compiler le projet avec toutes les fonctionnalités souhaitées, il faut disposer de :

- Un compilateur C++ compatible avec le projet.
- CMake.
- Qt avec le module Widgets pour l'interface graphique.
- PostgreSQL et sa bibliothèque cliente `libpq` pour la connexion à la base de données.
- Un environnement de compilation compatible avec la version de Qt installée.

### Compilation sous Windows

Ouvre un terminal dans le dossier du projet.

Configure CMake en indiquant les chemins d'installation de Qt et de PostgreSQL, en les adaptant à ton ordinateur :

```bat
cmake -S . -B build -G Ninja -DCMAKE_PREFIX_PATH="C:/chemin/vers/Qt" -DPOSTGRESQL_ROOT="C:/chemin/vers/PostgreSQL"
```

Compile ensuite les cibles disponibles :

```bat
cmake --build build
```

Si Qt ou PostgreSQL n'est pas détecté, certaines fonctionnalités optionnelles peuvent être désactivées par la configuration du projet.

### Lancer l'application graphique

Après une compilation réussie avec Qt :

```bat
build\smart_logistics_gui.exe
```

### Lancer l'application en console

```bat
build\smart_logistics.exe
```

### Exécuter les tests

```bat
build\smart_logistics_tests.exe
```

Le test de connexion PostgreSQL, lorsqu'il est disponible, peut être lancé séparément :

```bat
build\postgresql_test.exe
```

Ce dernier nécessite une base de données accessible et une configuration de connexion correcte.

## Configuration de la base de données

Le projet prévoit une intégration avec PostgreSQL pour stocker et consulter les données du système.

Pour préparer la base de données :

1. Installe et démarre PostgreSQL.
2. Crée la base de données utilisée par l'application.
3. Consulte `bdd.txt` pour préparer le schéma et les tables nécessaires.
4. Configure les paramètres de connexion dans l'environnement local du projet.
5. Lance le test de connexion PostgreSQL, si cette cible a été compilée.

**Sécurité :** ne publie jamais de mot de passe, de clé ou d'identifiant confidentiel dans le dépôt. Les paramètres sensibles doivent être conservés localement et exclus du contrôle de version.

## Concepts de programmation mis en pratique

Le projet permet notamment de mettre en œuvre :

- La programmation orientée objet : encapsulation, abstraction, héritage et polymorphisme.
- Les classes abstraites et les relations entre objets.
- Les patrons de conception : Factory, Repository, Strategy, Observer et State.
- Les graphes et l'algorithme de Dijkstra pour la recherche d'itinéraires.
- La programmation concurrente pour la surveillance de la flotte.
- La séparation entre logique métier, interface utilisateur et accès aux données.
- La gestion des exceptions et la validation des opérations.

## État du projet

SmartLogistics évolue progressivement vers une solution réunissant gestion métier, interface graphique et stockage relationnel.

Le dépôt contient l'application en console, les composants de l'interface graphique et le support de connexion à PostgreSQL. L'étendue de l'intégration entre ces composants et la persistance complète de chaque module doivent être vérifiées au fur et à mesure du développement.

## Objectifs

L'objectif est de proposer un outil centralisé qui facilite l'organisation des transports, améliore le suivi des livraisons, optimise l'utilisation des véhicules et aide à la prise de décision grâce aux informations et aux statistiques disponibles.

## Dépôt

Code source : [github.com/Bals24/SmartLogistics](https://github.com/Bals24/SmartLogistics)

