#pragma once

#include <QMainWindow>

/*
 * Déclarations anticipées des classes Qt.
 * Cela évite d'inclure inutilement tous les headers Qt dans ce fichier.
 */
class QLabel;
class QPushButton;
class QStackedWidget;
class QWidget;
class QTableWidget;
class QLineEdit;


/*
 * Fenêtre principale de l'application Smart Logistics.
 */
class MainWindow : public QMainWindow
{
public:
    explicit MainWindow(QWidget* parent = nullptr);

private:

    // ============================================================
    // Création des différentes parties de l'interface
    // ============================================================

    QWidget* createSidebar();
    QWidget* createTopBar();

    QWidget* createDashboard();
    QWidget* createCommandesPage();
    QWidget* createColisPage();
    QWidget* createClientsPage();
    QWidget* createVehiculesPage();
    QWidget* createChauffeursPage();
    QWidget* createEntrepotsPage();
    QWidget* createLivraisonsPage();
    QWidget* createItinerairesPage();
    QWidget* createIncidentsPage();
    QWidget* createMaintenancePage();
    QWidget* createStatistiquesPage();

    QPushButton* createMenuButton(const QString& text);

    // ============================================================
    // Navigation
    // ============================================================

    void changePage(int index);

    // ============================================================
    // Commandes
    // ============================================================

    void ajouterCommande();
    void modifierCommande();
    void supprimerCommande();
    void rechercherCommande();
    void actualiserCommandes();

    // ============================================================
    // Colis
    // ============================================================

    void ajouterColis();
    void modifierColis();
    void supprimerColis();
    void rechercherColis();
    void actualiserColis();

    // ============================================================
    // Clients
    // ============================================================

    void ajouterClient();
    void modifierClient();
    void supprimerClient();
    void rechercherClient();
    void actualiserClients();

    // ============================================================
    // Véhicules
    // ============================================================

    void ajouterVehicule();
    void modifierVehicule();
    void supprimerVehicule();
    void rechercherVehicule();
    void actualiserVehicules();

    // ============================================================
    // Chauffeurs
    // ============================================================

    void ajouterChauffeur();
    void modifierChauffeur();
    void supprimerChauffeur();
    void rechercherChauffeur();
    void actualiserChauffeurs();

    // ============================================================
    // Entrepôts
    // ============================================================

    void ajouterEntrepot();
    void modifierEntrepot();
    void supprimerEntrepot();
    void rechercherEntrepot();
    void actualiserEntrepots();

    // ============================================================
    // Livraisons
    // ============================================================

    void ajouterLivraison();
    void modifierLivraison();
    void supprimerLivraison();
    void rechercherLivraison();
    void actualiserLivraisons();

    // ============================================================
    // Itinéraires
    // ============================================================

    void ajouterItineraire();
    void modifierItineraire();
    void supprimerItineraire();
    void rechercherItineraire();
    void actualiserItineraires();

    // ============================================================
    // Incidents
    // ============================================================

    void ajouterIncident();
    void modifierIncident();
    void supprimerIncident();
    void rechercherIncident();
    void actualiserIncidents();

    // ============================================================
    // Maintenance
    // ============================================================

    void ajouterMaintenance();
    void modifierMaintenance();
    void supprimerMaintenance();
    void rechercherMaintenance();
    void actualiserMaintenances();

    // ============================================================
    // Statistiques
    // ============================================================

    void actualiserStatistiques();

    // ============================================================
    // Barre supérieure
    // ============================================================

    QLabel* pageTitle_;
    QLabel* pageSubtitle_;

    // ============================================================
    // Pages
    // ============================================================

    QStackedWidget* pages_;

    // ============================================================
    // Boutons du menu
    // ============================================================

    QPushButton* dashboardButton_;
    QPushButton* commandesButton_;
    QPushButton* colisButton_;
    QPushButton* clientsButton_;
    QPushButton* vehiculesButton_;
    QPushButton* chauffeursButton_;
    QPushButton* entrepotsButton_;
    QPushButton* livraisonsButton_;
    QPushButton* itinerairesButton_;
    QPushButton* incidentsButton_;
    QPushButton* maintenanceButton_;
    QPushButton* statistiquesButton_;

    // ============================================================
    // Tables
    // ============================================================

    QTableWidget* commandesTable_;
    QTableWidget* colisTable_;
    QTableWidget* clientsTable_;
    QTableWidget* vehiculesTable_;
    QTableWidget* chauffeursTable_;
    QTableWidget* entrepotsTable_;
    QTableWidget* livraisonsTable_;
    QTableWidget* itinerairesTable_;
    QTableWidget* incidentsTable_;
    QTableWidget* maintenanceTable_;

    // ============================================================
    // Champs de recherche
    // ============================================================

    QLineEdit* rechercheCommande_;
    QLineEdit* rechercheColis_;
    QLineEdit* rechercheClient_;
    QLineEdit* rechercheVehicule_;
    QLineEdit* rechercheChauffeur_;
    QLineEdit* rechercheEntrepot_;
    QLineEdit* rechercheLivraison_;
    QLineEdit* rechercheItineraire_;
    QLineEdit* rechercheIncident_;
    QLineEdit* rechercheMaintenance_;
};