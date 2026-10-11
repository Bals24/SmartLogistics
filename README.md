# Smart Logistics & Fleet Management

Systeme de gestion logistique et de flotte en C++20 oriente objet :
clients, commandes, colis, entrepots, vehicules, chauffeurs, itineraires,
livraisons, incidents, maintenance, statistiques, avec affectation
automatique des ressources et optimisation d'itineraire par graphe.

## Compiler et executer

```bash
cmake -S . -B build
cmake --build build -j
./build/smart_logistics
```

Ou directement avec g++ (si CMake n'est pas installe) :
```bash
g++ -std=c++20 -Wall -Wextra -Iinclude -pthread main.cpp $(find src -name "*.cpp") -o smart_logistics
./smart_logistics
```

### Tests

Un jeu de tests sans dependance externe (pas de GoogleTest) est compile
automatiquement dans une deuxieme cible :
```bash
./build/smart_logistics_tests
```

## Dependances

- **C++20**, une bibliotheque standard recente (GCC 11+/Clang 14+).
- **pthread** (deja standard sur Linux/macOS ; sous Windows, MinGW le fournit).
- **nlohmann/json** : header unique deja inclus dans `include/external/json.hpp`
  (aucune installation requise, aucun acces reseau necessaire pour compiler).

Rien d'autre a installer : le projet compile "out of the box" avec CMake ou
g++ directement.

## Structure du projet

```
SmartLogistics/
├── CMakeLists.txt
├── main.cpp                    Interface console (Niveau 7)
├── include/                    Headers, organises par responsabilite
│   ├── entities/                Client, Colis(+4), Vehicule(+4), Chauffeur,
│   │                             Commande, Entrepot, ZoneStockage, Livraison,
│   │                             Incident(+5), Maintenance, Enums
│   ├── etats/                   State Pattern : cycle de vie de la Commande
│   ├── observateurs/             Observer Pattern : notifications de statut
│   ├── strategies/               Strategy Pattern : ponderation d'itineraire
│   ├── graphe/                   Graphe de villes + Dijkstra generique
│   ├── factories/                Factory Pattern : Vehicule, Colis
│   ├── repositories/             Repository Pattern (template generique)
│   ├── services/                 GestionnaireFlotte, AllocateurRessources,
│   │                             ServiceMaintenance, ServiceStatistiques,
│   │                             SurveillanceFlotte (thread de fond)
│   ├── persistence/              Sauvegarde / chargement JSON
│   ├── utils/                    Exceptions metier, generateur d'ID
│   └── external/json.hpp         nlohmann/json (bundlee, header-only)
├── src/                          Implementations, meme arborescence
├── tests/test_main.cpp           Tests (assertions simples)
└── data/                         Fichiers JSON generes par la sauvegarde
```

## Ce que le projet demontre

### POO fondamentale
- **Abstraction** : `Colis`, `Vehicule`, `Incident`, `EtatCommande`,
  `StrategieItineraire` sont des classes abstraites (methodes virtuelles pures).
- **Heritage** : 4 sous-types de `Colis`, 4 de `Vehicule`, 5 d'`Incident`,
  11 etats concrets de `Commande`.
- **Polymorphisme** : la flotte et les colis sont manipules via
  `vector<unique_ptr<Base>>` ; chaque objet affiche/se comporte selon son
  type reel sans que l'appelant le connaisse.
- **Encapsulation** : attributs prives partout, mutateurs qui valident
  (ex : `Vehicule::chargerPoids` refuse de depasser la capacite).
- **Composition vs association** : une `Commande` **possede** ses `Colis`
  (`unique_ptr`) mais est seulement **associee** a un `Client` (pointeur
  non possedant) ; une `Livraison` **possede** ses `Incident`s mais est
  **associee** a la `Commande`/au `Vehicule`/au `Chauffeur`.

### Design Patterns (Niveau 5)
- **State** (`etats/`) : le cycle de vie de la `Commande`
  (CREEE -> ... -> LIVREE, avec ANNULEE et INCIDENT) est gere par des
  classes d'etat qui savent seules vers quel etat suivant transiter, et
  levent `TransitionInvalideException` sinon (ex : impossible d'annuler
  une commande EN_TRANSIT).
- **Observer** (`observateurs/`) : `NotificationClient` et
  `NotificationGestionnaire` sont notifies automatiquement a chaque
  changement de statut d'une commande.
- **Strategy** (`strategies/`) : trois facons de "peser" une route
  (`StrategiePlusCourtChemin`, `StrategiePlusRapide`, `StrategieMoinsCher`)
  injectees dans un Dijkstra generique unique.
- **Factory** (`factories/`) : `VehiculeFactory` et `ColisFactory`
  centralisent la creation polymorphe (utilisees par le menu console et
  par la persistance JSON pour reconstruire les objets a partir d'un
  champ `"type"`).
- **Repository** (`repositories/`) : `IRepository<T>` + implementation
  memoire generique (`RepositoryMemoire<T>`), instanciee pour chaque
  entite (`ClientRepository`, `VehiculeRepository`, ...). Le reste du
  code ne connait que l'interface, pas le stockage reel.

### Algorithmes (Niveau 3)
- Graphe de villes malgaches (liste d'adjacence) et **Dijkstra generique**
  parametre par la strategie de ponderation. Les routes `BLOQUEE` sont
  systematiquement ignorees (poids infini) ; les routes `RALENTIE`
  penalisent surtout la strategie "plus rapide".

### Persistance (Niveau 4, phase 1)
- Serialisation/deserialisation JSON (`nlohmann/json`) pour les clients,
  chauffeurs et la flotte (avec reconstruction polymorphe des vehicules
  via `VehiculeFactory`). Menu 14/15 pour sauvegarder/recharger.
- La persistance SQLite (phase 2 du cahier des charges) n'est pas
  implementee ici mais le `IRepository<T>` a ete concu precisement pour
  qu'on puisse ajouter une implementation SQLite derriere la meme
  interface sans toucher au reste du code.

### Concurrence (Niveau 6)
- `SurveillanceFlotte` fait tourner un `std::thread` en arriere-plan qui
  verifie periodiquement la flotte (`ServiceMaintenance::detecterEtMarquer`)
  pendant que le thread principal continue de faire tourner le menu
  console. Toutes les donnees partagees (`GestionnaireFlotte`,
  `ServiceMaintenance`) sont protegees par mutex interne.

### Interface (Niveau 7)
- Menu console complet (15 actions + quitter) qui relie tous les niveaux
  precedents : creation de commande, progression du cycle de vie,
  affectation automatique vehicule+chauffeur avec calcul d'itineraire,
  declaration d'incidents, verification de maintenance, statistiques,
  sauvegarde/chargement JSON.

## Limites connues / pistes d'evolution

- La persistance SQLite (section 22, phase 2) n'est pas implementee ;
  seul JSON (phase 1) l'est. L'interface `IRepository<T>` est prete pour
  recevoir une implementation SQLite en parallele de `RepositoryMemoire<T>`.
- Pas d'interface graphique (Qt/SFML) : le menu reste en console, comme
  prevu au Niveau 7 avant evolution eventuelle vers une GUI.
- Le score de l'`AllocateurRessources` est une heuristique simple
  (capacite "juste" + niveau de carburant) ; le cahier des charges
  suggere d'y integrer aussi la distance au depot et l'etat du vehicule,
  ce qui est une extension directe de `AllocateurRessources::calculerScore`.
