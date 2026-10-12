# Smart Logistics & Fleet Management

Système de gestion logistique et de flotte en C++20 orienté objet : clients,
commandes, colis, entrepôts, véhicules, chauffeurs, itinéraires, livraisons,
incidents, maintenance et statistiques, avec affectation automatique des
ressources et optimisation d'itinéraire par graphe.

Trois façons de l'utiliser, toutes branchées sur les mêmes données :

| Application | Exécutable | Données |
|---|---|---|
| Console (menu à 17 actions) | `smart_logistics` | mémoire, JSON, PostgreSQL |
| Interface graphique Qt 6 (12 pages) | `smart_logistics_gui` | PostgreSQL (ou démonstration hors ligne) |
| Tests | `smart_logistics_tests` | aucune |

## Compiler

```bash
cmake -S . -B build
cmake --build build -j
ctest --test-dir build        # lance les tests
```

Qt 6 et PostgreSQL sont **optionnels** : s'ils sont introuvables, CMake
l'indique et compile quand même la console et les tests.

```bash
cmake -S . -B build -DSL_WITH_GUI=OFF        # sans interface graphique
cmake -S . -B build -DSL_WITH_POSTGRES=OFF   # sans PostgreSQL
cmake -S . -B build -DPOSTGRESQL_ROOT="D:/PostgreSQL/18"   # Windows : autre dossier
```

### Dépendances

- **C++20** (GCC 11+, Clang 14+, MinGW récent) et **pthread**.
- **nlohmann/json** : déjà inclus (`include/external/json.hpp`).
- *Optionnel* — **PostgreSQL 14+** et sa bibliothèque cliente `libpq`
  (Linux : `sudo apt install libpq-dev`). Sous MinGW, la bibliothèque
  d'import `libpq.dll.a` est générée automatiquement depuis
  `third_party/postgresql/libpq.def` avec `dlltool`.
- *Optionnel* — **Qt 6** (module Widgets) pour l'interface graphique.

## Base de données PostgreSQL

1. Créer la base et charger le schéma :
   ```bash
   createdb -U postgres smart_logistics
   psql -U postgres -d smart_logistics -f bdd.txt
   ```
2. Définir le mot de passe **dans l'environnement** (jamais dans le code) :
   ```bash
   # Windows (cmd)             # Linux / macOS
   set SL_DB_PASSWORD=...      export SL_DB_PASSWORD=...
   ```
   Autres variables (valeurs par défaut) : `SL_DB_HOST` (`::1`),
   `SL_DB_PORT` (`5432`), `SL_DB_NAME` (`smart_logistics`),
   `SL_DB_USER` (`postgres`).
3. Aucune migration à lancer : l'application ajoute elle-même les colonnes
   dont elle a besoin aux tables de `bdd.txt` (`code`, `experience_annees`,
   `type_colis`, ...). L'opération est idempotente.

Test de connexion seul : `./build/postgresql_test`.

## Interface graphique

`./build/smart_logistics_gui` (sous Windows, lancez-la depuis Qt Creator ou
exécutez `windeployqt build\smart_logistics_gui.exe` pour copier les DLL Qt).

- **12 pages** : tableau de bord, commandes, colis, clients, véhicules,
  chauffeurs, entrepôts, livraisons, itinéraires, incidents, maintenance,
  statistiques.
- **Les données viennent de PostgreSQL** et chaque ajout, modification ou
  suppression est **enregistré automatiquement** (la barre du haut indique
  l'état de la base). Sans base, l'interface fonctionne avec des données de
  démonstration.
- **Ajouter / Modifier** ouvrent un formulaire (listes de choix, calendrier,
  champs obligatoires, unités).
- **Intégrité** : une suppression refusée par la base (ex : un client qui a
  des commandes) restaure la ligne et affiche la raison.
- **Tableau de bord et statistiques** sont calculés à partir des données
  affichées, donc toujours identiques à la base.
- **Itinéraires** (page Itinéraires) : « Calculer l'itinéraire » applique
  Dijkstra avec la stratégie choisie (plus court, plus rapide, moins cher)
  sur le réseau routier partagé avec la console et remplit distance, durée
  et trajet.
- **Fin de livraison** : « Terminer la livraison » libère véhicule et chauffeur
  et clôture la commande (et ses colis) quand toutes ses livraisons sont finies.
- **Cohérence entre pages** : une maintenance « En cours » immobilise le
  véhicule ; un incident ouvert met la livraison et la commande en « Incident »
  (et le véhicule « Indisponible » pour une panne ou un accident) ; tout est
  rétabli à la clôture.
- **Affectation automatique** (page Livraisons) : choisit le meilleur
  véhicule et chauffeur disponibles avec `AllocateurRessources` (capacité,
  froid, permis, carburant) et met à jour livraison, véhicule, chauffeur et
  commande.

## Structure du projet

```
SmartLogistics/
├── CMakeLists.txt
├── bdd.txt                     Schéma PostgreSQL et données d'exemple
├── main.cpp                    Application console (menu 17 actions)
├── include/                    Headers, organisés par responsabilité
│   ├── entities/                Client, Colis(+4), Vehicule(+4), Chauffeur,
│   │                             Commande, Entrepot, ZoneStockage, Livraison,
│   │                             Incident(+5), Maintenance, Enums
│   ├── etats/                   State : cycle de vie de la Commande
│   ├── observateurs/            Observer : notifications de statut
│   ├── strategies/              Strategy : pondération d'itinéraire
│   ├── graphe/                  Graphe de villes, Dijkstra générique, réseau routier
│   ├── factories/               Factory : Vehicule, Colis
│   ├── repositories/            Repository (template générique)
│   ├── services/                GestionnaireFlotte, AllocateurRessources,
│   │                             ServiceMaintenance, ServiceStatistiques,
│   │                             SurveillanceFlotte (thread de fond)
│   ├── persistence/             Sauvegarde / chargement JSON
│   ├── database/                Connexion PostgreSQL (libpq) et persistance
│   ├── utils/                   Exceptions métier, générateur d'ID
│   └── external/json.hpp        nlohmann/json (header-only)
├── src/                        Implémentations, même arborescence
│                                (src/database/ n'est compilé que si PostgreSQL est disponible)
├── gui/                        Interface Qt : MainWindow, GuiDatabase
│                                (tableaux <-> PostgreSQL), FormulaireDialog
├── tests/                      Tests sans dépendance externe
├── third_party/postgresql/     libpq.def (génération de libpq.dll.a sous MinGW)
└── data/                       Fichiers JSON générés par la sauvegarde
```

## Ce que le projet démontre

### POO fondamentale
- **Abstraction** : `Colis`, `Vehicule`, `Incident`, `EtatCommande`,
  `StrategieItineraire` sont des classes abstraites.
- **Héritage** : 4 sous-types de `Colis`, 4 de `Vehicule`, 5 d'`Incident`,
  11 états concrets de `Commande`.
- **Polymorphisme** : flotte et colis manipulés via `vector<unique_ptr<Base>>`.
- **Encapsulation** : attributs privés, mutateurs qui valident (ex :
  `Vehicule::chargerPoids` refuse de dépasser la capacité).
- **Composition vs association** : une `Commande` possède ses `Colis`
  (`unique_ptr`) mais n'est qu'associée à son `Client`.

### Design Patterns
- **State** : cycle de vie de la `Commande` (CREEE -> ... -> LIVREE, avec
  ANNULEE et INCIDENT) ; une transition interdite lève
  `TransitionInvalideException`.
- **Observer** : `NotificationClient` et `NotificationGestionnaire` sont
  notifiés à chaque changement de statut.
- **Strategy** : plus court chemin, plus rapide, moins cher, injectés dans un
  Dijkstra générique unique.
- **Factory** : `VehiculeFactory` et `ColisFactory` (menu console,
  persistance JSON/PostgreSQL, affectation automatique de l'interface).
- **Repository** : `IRepository<T>` + `RepositoryMemoire<T>` générique.

### Algorithmes
- Graphe de villes malgaches (liste d'adjacence) et **Dijkstra générique** ;
  les routes `BLOQUEE` sont ignorées, les routes `RALENTIE` pénalisent la
  stratégie « plus rapide ».
- **Affectation des ressources** : score d'un véhicule = taux de remplissage
  (70 %) + niveau de carburant (30 %), après filtrage par capacité, type
  (froid) et permis du chauffeur.

### Persistance
- **JSON** (`nlohmann/json`) : clients, chauffeurs et flotte, avec
  reconstruction polymorphe des véhicules (menus 14 et 15).
- **PostgreSQL** (`libpq`) : requêtes paramétrées (pas d'injection SQL),
  transactions. La console sauvegarde/charge clients, chauffeurs et flotte
  (menus 16 et 17) ; l'interface gère les 10 tables.

### Concurrence
- `SurveillanceFlotte` fait tourner un `std::thread` qui vérifie
  périodiquement la flotte pendant que le menu reste utilisable. Les données
  partagées sont protégées par mutex.

## Limites connues / pistes d'évolution

- La console ne persiste pas encore les commandes, colis, livraisons,
  incidents, entrepôts ni la maintenance dans PostgreSQL (l'interface, si).
- Le score d'affectation ne tient pas compte de la distance au dépôt ni de
  l'état du véhicule : extension directe de
  `AllocateurRessources::calculerScore`.