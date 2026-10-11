#include "MainWindow.hpp"
#include "Style.hpp"

#include <QFrame>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QStackedWidget>
#include <QStyle>
#include <QStringList>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>


// ================================================================
// CONSTRUCTEUR PRINCIPAL
// ================================================================

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      pageTitle_(nullptr),
      pageSubtitle_(nullptr),
      pages_(nullptr),

      dashboardButton_(nullptr),
      commandesButton_(nullptr),
      colisButton_(nullptr),
      clientsButton_(nullptr),
      vehiculesButton_(nullptr),
      chauffeursButton_(nullptr),
      entrepotsButton_(nullptr),
      livraisonsButton_(nullptr),
      itinerairesButton_(nullptr),
      incidentsButton_(nullptr),
      maintenanceButton_(nullptr),
      statistiquesButton_(nullptr),

      commandesTable_(nullptr),
      colisTable_(nullptr),
      clientsTable_(nullptr),
      vehiculesTable_(nullptr),
      chauffeursTable_(nullptr),
      entrepotsTable_(nullptr),
      livraisonsTable_(nullptr),
      itinerairesTable_(nullptr),
      incidentsTable_(nullptr),
      maintenanceTable_(nullptr),

      rechercheCommande_(nullptr),
      rechercheColis_(nullptr),
      rechercheClient_(nullptr),
      rechercheVehicule_(nullptr),
      rechercheChauffeur_(nullptr),
      rechercheEntrepot_(nullptr),
      rechercheLivraison_(nullptr),
      rechercheItineraire_(nullptr),
      rechercheIncident_(nullptr),
      rechercheMaintenance_(nullptr)
{
    // ------------------------------------------------------------
    // Configuration générale de la fenêtre
    // ------------------------------------------------------------

    setWindowTitle("Smart Logistics");
    resize(1400, 850);

    setStyleSheet(SmartLogisticsStyle::mainStyle());

    // ------------------------------------------------------------
    // Widget central
    // ------------------------------------------------------------

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // ------------------------------------------------------------
    // Sidebar
    // ------------------------------------------------------------

    QWidget* sidebar = createSidebar();

    // ------------------------------------------------------------
    // Zone principale
    // ------------------------------------------------------------

    QWidget* content = new QWidget();

    QVBoxLayout* contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(0);

    contentLayout->addWidget(createTopBar());

    // ------------------------------------------------------------
    // Système de pages
    // ------------------------------------------------------------

    pages_ = new QStackedWidget();

    pages_->addWidget(createDashboard());
    pages_->addWidget(createCommandesPage());
    pages_->addWidget(createColisPage());
    pages_->addWidget(createClientsPage());
    pages_->addWidget(createVehiculesPage());
    pages_->addWidget(createChauffeursPage());
    pages_->addWidget(createEntrepotsPage());
    pages_->addWidget(createLivraisonsPage());
    pages_->addWidget(createItinerairesPage());
    pages_->addWidget(createIncidentsPage());
    pages_->addWidget(createMaintenancePage());
    pages_->addWidget(createStatistiquesPage());

    contentLayout->addWidget(pages_);

    // ------------------------------------------------------------
    // Assemblage
    // ------------------------------------------------------------

    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(content);

    // ------------------------------------------------------------
    // Connexion des boutons du menu
    // ------------------------------------------------------------

    connect(dashboardButton_, &QPushButton::clicked,
            this, [this]() { changePage(0); });

    connect(commandesButton_, &QPushButton::clicked,
            this, [this]() { changePage(1); });

    connect(colisButton_, &QPushButton::clicked,
            this, [this]() { changePage(2); });

    connect(clientsButton_, &QPushButton::clicked,
            this, [this]() { changePage(3); });

    connect(vehiculesButton_, &QPushButton::clicked,
            this, [this]() { changePage(4); });

    connect(chauffeursButton_, &QPushButton::clicked,
            this, [this]() { changePage(5); });

    connect(entrepotsButton_, &QPushButton::clicked,
            this, [this]() { changePage(6); });

    connect(livraisonsButton_, &QPushButton::clicked,
            this, [this]() { changePage(7); });

    connect(itinerairesButton_, &QPushButton::clicked,
            this, [this]() { changePage(8); });

    connect(incidentsButton_, &QPushButton::clicked,
            this, [this]() { changePage(9); });

    connect(maintenanceButton_, &QPushButton::clicked,
            this, [this]() { changePage(10); });

    connect(statistiquesButton_, &QPushButton::clicked,
            this, [this]() { changePage(11); });

    // Page initiale
    changePage(0);
}


// ================================================================
// SIDEBAR
// ================================================================

QWidget* MainWindow::createSidebar()
{
    QWidget* sidebar = new QWidget();
    sidebar->setObjectName("sidebar");
    sidebar->setFixedWidth(245);

    QVBoxLayout* layout = new QVBoxLayout(sidebar);

    layout->setContentsMargins(15, 25, 15, 20);
    layout->setSpacing(6);

    QLabel* logo = new QLabel("SMART LOGISTICS");
    logo->setObjectName("logo");

    QLabel* subtitle = new QLabel("Fleet Management");
    subtitle->setObjectName("sidebarSubtitle");

    layout->addWidget(logo);
    layout->addWidget(subtitle);
    layout->addSpacing(25);

    // ------------------------------------------------------------
    // Boutons du menu
    // ------------------------------------------------------------

    dashboardButton_ = createMenuButton("Tableau de bord");
    commandesButton_ = createMenuButton("Commandes");
    colisButton_ = createMenuButton("Colis");
    clientsButton_ = createMenuButton("Clients");
    vehiculesButton_ = createMenuButton("Véhicules");
    chauffeursButton_ = createMenuButton("Chauffeurs");
    entrepotsButton_ = createMenuButton("Entrepôts");
    livraisonsButton_ = createMenuButton("Livraisons");
    itinerairesButton_ = createMenuButton("Itinéraires");
    incidentsButton_ = createMenuButton("Incidents");
    maintenanceButton_ = createMenuButton("Maintenance");
    statistiquesButton_ = createMenuButton("Statistiques");

    layout->addWidget(dashboardButton_);
    layout->addWidget(commandesButton_);
    layout->addWidget(colisButton_);
    layout->addWidget(clientsButton_);
    layout->addWidget(vehiculesButton_);
    layout->addWidget(chauffeursButton_);
    layout->addWidget(entrepotsButton_);
    layout->addWidget(livraisonsButton_);
    layout->addWidget(itinerairesButton_);
    layout->addWidget(incidentsButton_);
    layout->addWidget(maintenanceButton_);
    layout->addWidget(statistiquesButton_);

    layout->addStretch();

    QLabel* systemStatus =
        new QLabel("● Système opérationnel");

    systemStatus->setObjectName("systemStatus");

    layout->addWidget(systemStatus);

    return sidebar;
}


// ================================================================
// BARRE SUPÉRIEURE
// ================================================================

QWidget* MainWindow::createTopBar()
{
    QWidget* topBar = new QWidget();

    topBar->setObjectName("topBar");
    topBar->setFixedHeight(82);

    QHBoxLayout* layout = new QHBoxLayout(topBar);

    layout->setContentsMargins(30, 15, 30, 15);

    QVBoxLayout* titleLayout = new QVBoxLayout();

    titleLayout->setSpacing(2);

    pageTitle_ = new QLabel("Tableau de bord");
    pageTitle_->setObjectName("pageTitle");

    pageSubtitle_ =
        new QLabel("Vue générale de votre activité logistique");

    pageSubtitle_->setObjectName("pageSubtitle");

    titleLayout->addWidget(pageTitle_);
    titleLayout->addWidget(pageSubtitle_);

    layout->addLayout(titleLayout);
    layout->addStretch();

    QLabel* userLabel = new QLabel("Administrateur");
    userLabel->setObjectName("userLabel");

    layout->addWidget(userLabel);

    return topBar;
}


// ================================================================
// TABLEAU DE BORD
// ================================================================

QWidget* MainWindow::createDashboard()
{
    QWidget* page = new QWidget();

    QScrollArea* scrollArea = new QScrollArea();

    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    QWidget* content = new QWidget();

    QVBoxLayout* layout = new QVBoxLayout(content);

    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(25);

    QLabel* sectionTitle =
        new QLabel("Vue d'ensemble");

    sectionTitle->setObjectName("sectionTitle");

    layout->addWidget(sectionTitle);

    QHBoxLayout* statsLayout = new QHBoxLayout();

    statsLayout->setSpacing(15);

    // Fonction locale pour créer les cartes statistiques
    auto createCard =
        [](const QString& title,
           const QString& value,
           const QString& description)
    {
        QFrame* card = new QFrame();

        card->setObjectName("statCard");

        QVBoxLayout* cardLayout =
            new QVBoxLayout(card);

        cardLayout->setContentsMargins(18, 15, 18, 15);

        QLabel* cardTitle =
            new QLabel(title);

        cardTitle->setObjectName("cardTitle");

        QLabel* cardValue =
            new QLabel(value);

        cardValue->setObjectName("cardValue");

        QLabel* cardDescription =
            new QLabel(description);

        cardDescription->setObjectName("cardDescription");

        cardLayout->addWidget(cardTitle);
        cardLayout->addWidget(cardValue);
        cardLayout->addWidget(cardDescription);
        cardLayout->addStretch();

        return card;
    };

    statsLayout->addWidget(
        createCard(
            "Commandes",
            "248",
            "Commandes enregistrées"
        )
    );

    statsLayout->addWidget(
        createCard(
            "Livraisons",
            "186",
            "Livraisons effectuées"
        )
    );

    statsLayout->addWidget(
        createCard(
            "En cours",
            "42",
            "Opérations en cours"
        )
    );

    statsLayout->addWidget(
        createCard(
            "Incidents",
            "7",
            "Incidents à traiter"
        )
    );

    layout->addLayout(statsLayout);

    QLabel* deliveriesTitle =
        new QLabel("Livraisons récentes");

    deliveriesTitle->setObjectName("sectionTitle");

    layout->addWidget(deliveriesTitle);

    QTableWidget* table =
        new QTableWidget();

    table->setObjectName("dashboardTable");

    table->setColumnCount(5);

    table->setHorizontalHeaderLabels({
        "Commande",
        "Client",
        "Destination",
        "Statut",
        "Date"
    });

    table->setRowCount(5);

    QStringList rows = {
        "#CMD-001|Client A|Antananarivo|Livrée|22/09/2026",
        "#CMD-002|Client B|Toamasina|En cours|22/09/2026",
        "#CMD-003|Client C|Fianarantsoa|En transit|21/09/2026",
        "#CMD-004|Client D|Mahajanga|Livrée|21/09/2026",
        "#CMD-005|Client E|Antsirabe|Incident|20/09/2026"
    };

    for (int row = 0; row < rows.size(); ++row)
    {
        QStringList values =
            rows[row].split("|");

        for (int column = 0;
             column < values.size();
             ++column)
        {
            table->setItem(
                row,
                column,
                new QTableWidgetItem(values[column])
            );
        }
    }

    table->horizontalHeader()
        ->setSectionResizeMode(QHeaderView::Stretch);

    table->setEditTriggers(
        QAbstractItemView::NoEditTriggers
    );

    table->setSelectionBehavior(
        QAbstractItemView::SelectRows
    );

    table->setAlternatingRowColors(true);

    table->setMinimumHeight(260);

    layout->addWidget(table);

    QLabel* fleetTitle =
        new QLabel("État de la flotte");

    fleetTitle->setObjectName("sectionTitle");

    layout->addWidget(fleetTitle);

    QHBoxLayout* fleetLayout =
        new QHBoxLayout();

    fleetLayout->setSpacing(15);

    fleetLayout->addWidget(
        createCard(
            "Véhicules actifs",
            "32",
            "En circulation"
        )
    );

    fleetLayout->addWidget(
        createCard(
            "En maintenance",
            "5",
            "Véhicules immobilisés"
        )
    );

    fleetLayout->addWidget(
        createCard(
            "Disponibles",
            "12",
            "Prêts à être affectés"
        )
    );

    layout->addLayout(fleetLayout);
    layout->addStretch();

    scrollArea->setWidget(content);

    QVBoxLayout* pageLayout =
        new QVBoxLayout(page);

    pageLayout->setContentsMargins(0, 0, 0, 0);

    pageLayout->addWidget(scrollArea);

    return page;
}


// ================================================================
// FONCTION UTILITAIRE POUR LES PAGES CRUD
// ================================================================

static void configurerTable(QTableWidget* table)
{
    table->setObjectName("dashboardTable");

    table->horizontalHeader()
        ->setSectionResizeMode(QHeaderView::Stretch);

    table->setSelectionBehavior(
        QAbstractItemView::SelectRows
    );

    table->setSelectionMode(
        QAbstractItemView::SingleSelection
    );

    table->setEditTriggers(
        QAbstractItemView::NoEditTriggers
    );

    table->setAlternatingRowColors(true);
}


// ================================================================
// PAGE COMMANDES
// ================================================================

QWidget* MainWindow::createCommandesPage()
{
    QWidget* page = new QWidget();

    QVBoxLayout* layout =
        new QVBoxLayout(page);

    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(20);

    QLabel* title =
        new QLabel("Gestion des commandes");

    title->setObjectName("sectionTitle");

    layout->addWidget(title);

    QHBoxLayout* buttons =
        new QHBoxLayout();

    buttons->addStretch();

    QPushButton* ajouter =
        new QPushButton("+ Ajouter");

    QPushButton* modifier =
        new QPushButton("Modifier");

    QPushButton* supprimer =
        new QPushButton("Supprimer");

    buttons->addWidget(ajouter);
    buttons->addWidget(modifier);
    buttons->addWidget(supprimer);

    layout->addLayout(buttons);

    QHBoxLayout* search =
        new QHBoxLayout();

    rechercheCommande_ =
        new QLineEdit();

    rechercheCommande_->setPlaceholderText(
        "Rechercher une commande..."
    );

    QPushButton* rechercher =
        new QPushButton("Rechercher");

    QPushButton* actualiser =
        new QPushButton("Actualiser");

    search->addWidget(rechercheCommande_);
    search->addWidget(rechercher);
    search->addWidget(actualiser);

    layout->addLayout(search);

    commandesTable_ =
        new QTableWidget();

    commandesTable_->setColumnCount(6);

    commandesTable_->setHorizontalHeaderLabels({
        "Commande",
        "Client",
        "Destination",
        "Statut",
        "Date",
        "Montant"
    });

    QStringList data = {
        "#CMD-001|Client A|Antananarivo|Livrée|22/09/2026|125 000 Ar",
        "#CMD-002|Client B|Toamasina|En cours|22/09/2026|280 000 Ar",
        "#CMD-003|Client C|Fianarantsoa|En transit|21/09/2026|175 000 Ar",
        "#CMD-004|Client D|Mahajanga|Livrée|21/09/2026|320 000 Ar",
        "#CMD-005|Client E|Antsirabe|Incident|20/09/2026|150 000 Ar"
    };

    commandesTable_->setRowCount(data.size());

    for (int r = 0; r < data.size(); ++r)
    {
        QStringList values = data[r].split("|");

        for (int c = 0; c < values.size(); ++c)
        {
            commandesTable_->setItem(
                r,
                c,
                new QTableWidgetItem(values[c])
            );
        }
    }

    configurerTable(commandesTable_);

    layout->addWidget(commandesTable_);

    connect(ajouter, &QPushButton::clicked,
            this, [this]() { ajouterCommande(); });

    connect(modifier, &QPushButton::clicked,
            this, [this]() { modifierCommande(); });

    connect(supprimer, &QPushButton::clicked,
            this, [this]() { supprimerCommande(); });

    connect(rechercher, &QPushButton::clicked,
            this, [this]() { rechercherCommande(); });

    connect(actualiser, &QPushButton::clicked,
            this, [this]() { actualiserCommandes(); });

    return page;
}


// ================================================================
// PAGE COLIS
// ================================================================

QWidget* MainWindow::createColisPage()
{
    QWidget* page = new QWidget();

    QVBoxLayout* layout =
        new QVBoxLayout(page);

    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(20);

    QLabel* title =
        new QLabel("Gestion des colis");

    title->setObjectName("sectionTitle");

    layout->addWidget(title);

    QHBoxLayout* buttons =
        new QHBoxLayout();

    buttons->addStretch();

    QPushButton* ajouter =
        new QPushButton("+ Ajouter");

    QPushButton* modifier =
        new QPushButton("Modifier");

    QPushButton* supprimer =
        new QPushButton("Supprimer");

    buttons->addWidget(ajouter);
    buttons->addWidget(modifier);
    buttons->addWidget(supprimer);

    layout->addLayout(buttons);

    QHBoxLayout* search =
        new QHBoxLayout();

    rechercheColis_ =
        new QLineEdit();

    rechercheColis_->setPlaceholderText(
        "Rechercher un colis..."
    );

    QPushButton* rechercher =
        new QPushButton("Rechercher");

    QPushButton* actualiser =
        new QPushButton("Actualiser");

    search->addWidget(rechercheColis_);
    search->addWidget(rechercher);
    search->addWidget(actualiser);

    layout->addLayout(search);

    colisTable_ =
        new QTableWidget();

    colisTable_->setColumnCount(7);

    colisTable_->setHorizontalHeaderLabels({
        "ID Colis",
        "Commande",
        "Type",
        "Poids",
        "Statut",
        "Destination",
        "Date"
    });

    QStringList data = {
        "COL-001|CMD-001|Standard|2.5 kg|Livré|Antananarivo|22/09/2026",
        "COL-002|CMD-002|Fragile|5.2 kg|En transit|Toamasina|22/09/2026",
        "COL-003|CMD-003|Standard|1.8 kg|En cours|Fianarantsoa|21/09/2026",
        "COL-004|CMD-004|Fragile|8.0 kg|Livré|Mahajanga|21/09/2026",
        "COL-005|CMD-005|Standard|3.4 kg|Incident|Antsirabe|20/09/2026"
    };

    colisTable_->setRowCount(data.size());

    for (int r = 0; r < data.size(); ++r)
    {
        QStringList values = data[r].split("|");

        for (int c = 0; c < values.size(); ++c)
        {
            colisTable_->setItem(
                r,
                c,
                new QTableWidgetItem(values[c])
            );
        }
    }

    configurerTable(colisTable_);

    layout->addWidget(colisTable_);

    connect(ajouter, &QPushButton::clicked,
            this, [this]() { ajouterColis(); });

    connect(modifier, &QPushButton::clicked,
            this, [this]() { modifierColis(); });

    connect(supprimer, &QPushButton::clicked,
            this, [this]() { supprimerColis(); });

    connect(rechercher, &QPushButton::clicked,
            this, [this]() { rechercherColis(); });

    connect(actualiser, &QPushButton::clicked,
            this, [this]() { actualiserColis(); });

    return page;
}


// ================================================================
// PAGE CLIENTS
// ================================================================

QWidget* MainWindow::createClientsPage()
{
    QWidget* page = new QWidget();

    QVBoxLayout* layout = new QVBoxLayout(page);

    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(20);

    QLabel* title =
        new QLabel("Gestion des clients");

    title->setObjectName("sectionTitle");

    layout->addWidget(title);

    QHBoxLayout* buttons = new QHBoxLayout();
    buttons->addStretch();

    QPushButton* ajouter =
        new QPushButton("+ Ajouter");

    QPushButton* modifier =
        new QPushButton("Modifier");

    QPushButton* supprimer =
        new QPushButton("Supprimer");

    buttons->addWidget(ajouter);
    buttons->addWidget(modifier);
    buttons->addWidget(supprimer);

    layout->addLayout(buttons);

    QHBoxLayout* search = new QHBoxLayout();

    rechercheClient_ = new QLineEdit();
    rechercheClient_->setPlaceholderText(
        "Rechercher un client..."
    );

    QPushButton* rechercher =
        new QPushButton("Rechercher");

    QPushButton* actualiser =
        new QPushButton("Actualiser");

    search->addWidget(rechercheClient_);
    search->addWidget(rechercher);
    search->addWidget(actualiser);

    layout->addLayout(search);

    clientsTable_ = new QTableWidget();

    clientsTable_->setColumnCount(6);

    clientsTable_->setHorizontalHeaderLabels({
        "ID",
        "Nom",
        "Téléphone",
        "Email",
        "Ville",
        "Commandes"
    });

    QStringList data = {
        "CLI-001|Client A|034 00 000 01|clienta@mail.com|Antananarivo|12",
        "CLI-002|Client B|034 00 000 02|clientb@mail.com|Toamasina|8",
        "CLI-003|Client C|034 00 000 03|clientc@mail.com|Fianarantsoa|15",
        "CLI-004|Client D|034 00 000 04|clientd@mail.com|Mahajanga|6",
        "CLI-005|Client E|034 00 000 05|cliente@mail.com|Antsirabe|10"
    };

    clientsTable_->setRowCount(data.size());

    for (int r = 0; r < data.size(); ++r)
    {
        QStringList values = data[r].split("|");

        for (int c = 0; c < values.size(); ++c)
        {
            clientsTable_->setItem(
                r,
                c,
                new QTableWidgetItem(values[c])
            );
        }
    }

    configurerTable(clientsTable_);

    layout->addWidget(clientsTable_);

    connect(ajouter, &QPushButton::clicked,
            this, [this]() { ajouterClient(); });

    connect(modifier, &QPushButton::clicked,
            this, [this]() { modifierClient(); });

    connect(supprimer, &QPushButton::clicked,
            this, [this]() { supprimerClient(); });

    connect(rechercher, &QPushButton::clicked,
            this, [this]() { rechercherClient(); });

    connect(actualiser, &QPushButton::clicked,
            this, [this]() { actualiserClients(); });

    return page;
}


// ================================================================
// PAGE VÉHICULES
// ================================================================

QWidget* MainWindow::createVehiculesPage()
{
    QWidget* page = new QWidget();

    QVBoxLayout* layout = new QVBoxLayout(page);

    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(20);

    QLabel* title =
        new QLabel("Gestion des véhicules");

    title->setObjectName("sectionTitle");

    layout->addWidget(title);

    QHBoxLayout* buttons = new QHBoxLayout();
    buttons->addStretch();

    QPushButton* ajouter =
        new QPushButton("+ Ajouter");

    QPushButton* modifier =
        new QPushButton("Modifier");

    QPushButton* supprimer =
        new QPushButton("Supprimer");

    buttons->addWidget(ajouter);
    buttons->addWidget(modifier);
    buttons->addWidget(supprimer);

    layout->addLayout(buttons);

    QHBoxLayout* search = new QHBoxLayout();

    rechercheVehicule_ = new QLineEdit();
    rechercheVehicule_->setPlaceholderText(
        "Rechercher un véhicule..."
    );

    QPushButton* rechercher =
        new QPushButton("Rechercher");

    QPushButton* actualiser =
        new QPushButton("Actualiser");

    search->addWidget(rechercheVehicule_);
    search->addWidget(rechercher);
    search->addWidget(actualiser);

    layout->addLayout(search);

    vehiculesTable_ = new QTableWidget();

    vehiculesTable_->setColumnCount(7);

    vehiculesTable_->setHorizontalHeaderLabels({
        "ID",
        "Immatriculation",
        "Marque",
        "Modèle",
        "Type",
        "Statut",
        "Kilométrage"
    });

    QStringList data = {
        "VEH-001|1234 TAA|Toyota|Hiace|Utilitaire|Actif|85 200 km",
        "VEH-002|5678 TAB|Renault|Master|Fourgon|Actif|62 500 km",
        "VEH-003|9012 TAC|Ford|Transit|Fourgon|Maintenance|120 300 km",
        "VEH-004|3456 TAD|Mercedes|Sprinter|Utilitaire|Actif|74 800 km",
        "VEH-005|7890 TAE|Peugeot|Boxer|Fourgon|Disponible|51 200 km"
    };

    vehiculesTable_->setRowCount(data.size());

    for (int r = 0; r < data.size(); ++r)
    {
        QStringList values = data[r].split("|");

        for (int c = 0; c < values.size(); ++c)
        {
            vehiculesTable_->setItem(
                r,
                c,
                new QTableWidgetItem(values[c])
            );
        }
    }

    configurerTable(vehiculesTable_);

    layout->addWidget(vehiculesTable_);

    connect(ajouter, &QPushButton::clicked,
            this, [this]() { ajouterVehicule(); });

    connect(modifier, &QPushButton::clicked,
            this, [this]() { modifierVehicule(); });

    connect(supprimer, &QPushButton::clicked,
            this, [this]() { supprimerVehicule(); });

    connect(rechercher, &QPushButton::clicked,
            this, [this]() { rechercherVehicule(); });

    connect(actualiser, &QPushButton::clicked,
            this, [this]() { actualiserVehicules(); });

    return page;
}


// ================================================================
// PAGE CHAUFFEURS
// ================================================================

QWidget* MainWindow::createChauffeursPage()
{
    QWidget* page = new QWidget();

    QVBoxLayout* layout = new QVBoxLayout(page);

    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(20);

    QLabel* title =
        new QLabel("Gestion des chauffeurs");

    title->setObjectName("sectionTitle");

    layout->addWidget(title);

    QHBoxLayout* buttons = new QHBoxLayout();
    buttons->addStretch();

    QPushButton* ajouter =
        new QPushButton("+ Ajouter");

    QPushButton* modifier =
        new QPushButton("Modifier");

    QPushButton* supprimer =
        new QPushButton("Supprimer");

    buttons->addWidget(ajouter);
    buttons->addWidget(modifier);
    buttons->addWidget(supprimer);

    layout->addLayout(buttons);

    QHBoxLayout* search = new QHBoxLayout();

    rechercheChauffeur_ = new QLineEdit();
    rechercheChauffeur_->setPlaceholderText(
        "Rechercher un chauffeur..."
    );

    QPushButton* rechercher =
        new QPushButton("Rechercher");

    QPushButton* actualiser =
        new QPushButton("Actualiser");

    search->addWidget(rechercheChauffeur_);
    search->addWidget(rechercher);
    search->addWidget(actualiser);

    layout->addLayout(search);

    chauffeursTable_ = new QTableWidget();

    chauffeursTable_->setColumnCount(6);

    chauffeursTable_->setHorizontalHeaderLabels({
        "ID",
        "Nom",
        "Téléphone",
        "Permis",
        "Véhicule",
        "Statut"
    });

    QStringList data = {
        "CHA-001|Chauffeur A|034 11 111 01|B|VEH-001|Disponible",
        "CHA-002|Chauffeur B|034 11 111 02|C|VEH-002|En livraison",
        "CHA-003|Chauffeur C|034 11 111 03|B|VEH-003|Indisponible",
        "CHA-004|Chauffeur D|034 11 111 04|C|VEH-004|Disponible",
        "CHA-005|Chauffeur E|034 11 111 05|B|VEH-005|En livraison"
    };

    chauffeursTable_->setRowCount(data.size());

    for (int r = 0; r < data.size(); ++r)
    {
        QStringList values = data[r].split("|");

        for (int c = 0; c < values.size(); ++c)
        {
            chauffeursTable_->setItem(
                r,
                c,
                new QTableWidgetItem(values[c])
            );
        }
    }

    configurerTable(chauffeursTable_);

    layout->addWidget(chauffeursTable_);

    connect(ajouter, &QPushButton::clicked,
            this, [this]() { ajouterChauffeur(); });

    connect(modifier, &QPushButton::clicked,
            this, [this]() { modifierChauffeur(); });

    connect(supprimer, &QPushButton::clicked,
            this, [this]() { supprimerChauffeur(); });

    connect(rechercher, &QPushButton::clicked,
            this, [this]() { rechercherChauffeur(); });

    connect(actualiser, &QPushButton::clicked,
            this, [this]() { actualiserChauffeurs(); });

    return page;
}


// ================================================================
// PAGE ENTREPÔTS
// ================================================================

QWidget* MainWindow::createEntrepotsPage()
{
    QWidget* page = new QWidget();

    QVBoxLayout* layout = new QVBoxLayout(page);

    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(20);

    QLabel* title =
        new QLabel("Gestion des entrepôts");

    title->setObjectName("sectionTitle");

    layout->addWidget(title);

    QHBoxLayout* buttons = new QHBoxLayout();
    buttons->addStretch();

    QPushButton* ajouter =
        new QPushButton("+ Ajouter");

    QPushButton* modifier =
        new QPushButton("Modifier");

    QPushButton* supprimer =
        new QPushButton("Supprimer");

    buttons->addWidget(ajouter);
    buttons->addWidget(modifier);
    buttons->addWidget(supprimer);

    layout->addLayout(buttons);

    QHBoxLayout* search = new QHBoxLayout();

    rechercheEntrepot_ = new QLineEdit();
    rechercheEntrepot_->setPlaceholderText(
        "Rechercher un entrepôt..."
    );

    QPushButton* rechercher =
        new QPushButton("Rechercher");

    QPushButton* actualiser =
        new QPushButton("Actualiser");

    search->addWidget(rechercheEntrepot_);
    search->addWidget(rechercher);
    search->addWidget(actualiser);

    layout->addLayout(search);

    entrepotsTable_ = new QTableWidget();

    entrepotsTable_->setColumnCount(6);

    entrepotsTable_->setHorizontalHeaderLabels({
        "ID",
        "Nom",
        "Ville",
        "Capacité",
        "Occupation",
        "Statut"
    });

    QStringList data = {
        "ENT-001|Entrepôt Central|Antananarivo|500 colis|320 colis|Actif",
        "ENT-002|Entrepôt Est|Toamasina|350 colis|210 colis|Actif",
        "ENT-003|Entrepôt Sud|Fianarantsoa|250 colis|180 colis|Actif",
        "ENT-004|Entrepôt Nord|Mahajanga|300 colis|120 colis|Disponible",
        "ENT-005|Entrepôt Vakinankaratra|Antsirabe|280 colis|240 colis|Presque plein"
    };

    entrepotsTable_->setRowCount(data.size());

    for (int r = 0; r < data.size(); ++r)
    {
        QStringList values = data[r].split("|");

        for (int c = 0; c < values.size(); ++c)
        {
            entrepotsTable_->setItem(
                r,
                c,
                new QTableWidgetItem(values[c])
            );
        }
    }

    configurerTable(entrepotsTable_);

    layout->addWidget(entrepotsTable_);

    connect(ajouter, &QPushButton::clicked,
            this, [this]() { ajouterEntrepot(); });

    connect(modifier, &QPushButton::clicked,
            this, [this]() { modifierEntrepot(); });

    connect(supprimer, &QPushButton::clicked,
            this, [this]() { supprimerEntrepot(); });

    connect(rechercher, &QPushButton::clicked,
            this, [this]() { rechercherEntrepot(); });

    connect(actualiser, &QPushButton::clicked,
            this, [this]() { actualiserEntrepots(); });

    return page;
}


// ================================================================
// PAGE LIVRAISONS
// ================================================================

QWidget* MainWindow::createLivraisonsPage()
{
    QWidget* page = new QWidget();

    QVBoxLayout* layout = new QVBoxLayout(page);

    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(20);

    QLabel* title =
        new QLabel("Gestion des livraisons");

    title->setObjectName("sectionTitle");

    layout->addWidget(title);

    QHBoxLayout* buttons = new QHBoxLayout();
    buttons->addStretch();

    QPushButton* ajouter =
        new QPushButton("+ Ajouter");

    QPushButton* modifier =
        new QPushButton("Modifier");

    QPushButton* supprimer =
        new QPushButton("Supprimer");

    buttons->addWidget(ajouter);
    buttons->addWidget(modifier);
    buttons->addWidget(supprimer);

    layout->addLayout(buttons);

    QHBoxLayout* search = new QHBoxLayout();

    rechercheLivraison_ = new QLineEdit();
    rechercheLivraison_->setPlaceholderText(
        "Rechercher une livraison..."
    );

    QPushButton* rechercher =
        new QPushButton("Rechercher");

    QPushButton* actualiser =
        new QPushButton("Actualiser");

    search->addWidget(rechercheLivraison_);
    search->addWidget(rechercher);
    search->addWidget(actualiser);

    layout->addLayout(search);

    livraisonsTable_ = new QTableWidget();

    livraisonsTable_->setColumnCount(7);

    livraisonsTable_->setHorizontalHeaderLabels({
        "ID",
        "Commande",
        "Client",
        "Chauffeur",
        "Véhicule",
        "Destination",
        "Statut"
    });

    QStringList data = {
        "LIV-001|CMD-001|Client A|CHA-001|VEH-001|Antananarivo|Livrée",
        "LIV-002|CMD-002|Client B|CHA-002|VEH-002|Toamasina|En cours",
        "LIV-003|CMD-003|Client C|CHA-004|VEH-004|Fianarantsoa|En transit",
        "LIV-004|CMD-004|Client D|CHA-005|VEH-005|Mahajanga|Livrée",
        "LIV-005|CMD-005|Client E|CHA-003|VEH-003|Antsirabe|Incident"
    };

    livraisonsTable_->setRowCount(data.size());

    for (int r = 0; r < data.size(); ++r)
    {
        QStringList values = data[r].split("|");

        for (int c = 0; c < values.size(); ++c)
        {
            livraisonsTable_->setItem(
                r,
                c,
                new QTableWidgetItem(values[c])
            );
        }
    }

    configurerTable(livraisonsTable_);

    layout->addWidget(livraisonsTable_);

    connect(ajouter, &QPushButton::clicked,
            this, [this]() { ajouterLivraison(); });

    connect(modifier, &QPushButton::clicked,
            this, [this]() { modifierLivraison(); });

    connect(supprimer, &QPushButton::clicked,
            this, [this]() { supprimerLivraison(); });

    connect(rechercher, &QPushButton::clicked,
            this, [this]() { rechercherLivraison(); });

    connect(actualiser, &QPushButton::clicked,
            this, [this]() { actualiserLivraisons(); });

    return page;
}


// ================================================================
// PAGE ITINÉRAIRES
// ================================================================

QWidget* MainWindow::createItinerairesPage()
{
    QWidget* page = new QWidget();

    QVBoxLayout* layout = new QVBoxLayout(page);

    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(20);

    QLabel* title =
        new QLabel("Gestion des itinéraires");

    title->setObjectName("sectionTitle");

    layout->addWidget(title);

    QHBoxLayout* buttons = new QHBoxLayout();
    buttons->addStretch();

    QPushButton* ajouter =
        new QPushButton("+ Ajouter");

    QPushButton* modifier =
        new QPushButton("Modifier");

    QPushButton* supprimer =
        new QPushButton("Supprimer");

    buttons->addWidget(ajouter);
    buttons->addWidget(modifier);
    buttons->addWidget(supprimer);

    layout->addLayout(buttons);

    QHBoxLayout* search = new QHBoxLayout();

    rechercheItineraire_ = new QLineEdit();

    rechercheItineraire_->setPlaceholderText(
        "Rechercher un itinéraire..."
    );

    QPushButton* rechercher =
        new QPushButton("Rechercher");

    QPushButton* actualiser =
        new QPushButton("Actualiser");

    search->addWidget(rechercheItineraire_);
    search->addWidget(rechercher);
    search->addWidget(actualiser);

    layout->addLayout(search);

    itinerairesTable_ = new QTableWidget();

    itinerairesTable_->setColumnCount(6);

    itinerairesTable_->setHorizontalHeaderLabels({
        "ID",
        "Départ",
        "Destination",
        "Distance",
        "Durée",
        "Statut"
    });

    QStringList data = {
        "ITI-001|Antananarivo|Toamasina|353 km|8h 30|Optimisé",
        "ITI-002|Antananarivo|Fianarantsoa|410 km|9h 15|Optimisé",
        "ITI-003|Antananarivo|Mahajanga|570 km|12h|Planifié",
        "ITI-004|Antananarivo|Antsirabe|170 km|4h|Actif",
        "ITI-005|Toamasina|Antananarivo|353 km|8h 30|Actif"
    };

    itinerairesTable_->setRowCount(data.size());

    for (int r = 0; r < data.size(); ++r)
    {
        QStringList values = data[r].split("|");

        for (int c = 0; c < values.size(); ++c)
        {
            itinerairesTable_->setItem(
                r,
                c,
                new QTableWidgetItem(values[c])
            );
        }
    }

    configurerTable(itinerairesTable_);

    layout->addWidget(itinerairesTable_);

    connect(ajouter, &QPushButton::clicked,
            this, [this]() { ajouterItineraire(); });

    connect(modifier, &QPushButton::clicked,
            this, [this]() { modifierItineraire(); });

    connect(supprimer, &QPushButton::clicked,
            this, [this]() { supprimerItineraire(); });

    connect(rechercher, &QPushButton::clicked,
            this, [this]() { rechercherItineraire(); });

    connect(actualiser, &QPushButton::clicked,
            this, [this]() { actualiserItineraires(); });

    return page;
}


// ================================================================
// PAGE INCIDENTS
// ================================================================

QWidget* MainWindow::createIncidentsPage()
{
    QWidget* page = new QWidget();

    QVBoxLayout* layout = new QVBoxLayout(page);

    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(20);

    QLabel* title =
        new QLabel("Gestion des incidents");

    title->setObjectName("sectionTitle");

    layout->addWidget(title);

    QHBoxLayout* buttons = new QHBoxLayout();
    buttons->addStretch();

    QPushButton* ajouter =
        new QPushButton("+ Ajouter");

    QPushButton* modifier =
        new QPushButton("Modifier");

    QPushButton* supprimer =
        new QPushButton("Supprimer");

    buttons->addWidget(ajouter);
    buttons->addWidget(modifier);
    buttons->addWidget(supprimer);

    layout->addLayout(buttons);

    QHBoxLayout* search = new QHBoxLayout();

    rechercheIncident_ = new QLineEdit();

    rechercheIncident_->setPlaceholderText(
        "Rechercher un incident..."
    );

    QPushButton* rechercher =
        new QPushButton("Rechercher");

    QPushButton* actualiser =
        new QPushButton("Actualiser");

    search->addWidget(rechercheIncident_);
    search->addWidget(rechercher);
    search->addWidget(actualiser);

    layout->addLayout(search);

    incidentsTable_ = new QTableWidget();

    incidentsTable_->setColumnCount(7);

    incidentsTable_->setHorizontalHeaderLabels({
        "ID",
        "Date",
        "Type",
        "Véhicule",
        "Chauffeur",
        "Description",
        "Statut"
    });

    QStringList data = {
        "INC-001|20/09/2026|Panne|VEH-003|CHA-003|Problème moteur|Ouvert",
        "INC-002|18/09/2026|Accident|VEH-002|CHA-002|Collision légère|Traité",
        "INC-003|17/09/2026|Retard|VEH-001|CHA-001|Trafic important|Clos",
        "INC-004|15/09/2026|Panne|VEH-005|CHA-005|Pneu crevé|Traité",
        "INC-005|12/09/2026|Retard|VEH-004|CHA-004|Conditions météo|Clos"
    };

    incidentsTable_->setRowCount(data.size());

    for (int r = 0; r < data.size(); ++r)
    {
        QStringList values = data[r].split("|");

        for (int c = 0; c < values.size(); ++c)
        {
            incidentsTable_->setItem(
                r,
                c,
                new QTableWidgetItem(values[c])
            );
        }
    }

    configurerTable(incidentsTable_);

    layout->addWidget(incidentsTable_);

    connect(ajouter, &QPushButton::clicked,
            this, [this]() { ajouterIncident(); });

    connect(modifier, &QPushButton::clicked,
            this, [this]() { modifierIncident(); });

    connect(supprimer, &QPushButton::clicked,
            this, [this]() { supprimerIncident(); });

    connect(rechercher, &QPushButton::clicked,
            this, [this]() { rechercherIncident(); });

    connect(actualiser, &QPushButton::clicked,
            this, [this]() { actualiserIncidents(); });

    return page;
}


// ================================================================
// PAGE MAINTENANCE
// ================================================================

QWidget* MainWindow::createMaintenancePage()
{
    QWidget* page = new QWidget();

    QVBoxLayout* layout = new QVBoxLayout(page);

    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(20);

    QLabel* title =
        new QLabel("Gestion de la maintenance");

    title->setObjectName("sectionTitle");

    layout->addWidget(title);

    QHBoxLayout* buttons = new QHBoxLayout();
    buttons->addStretch();

    QPushButton* ajouter =
        new QPushButton("+ Ajouter");

    QPushButton* modifier =
        new QPushButton("Modifier");

    QPushButton* supprimer =
        new QPushButton("Supprimer");

    buttons->addWidget(ajouter);
    buttons->addWidget(modifier);
    buttons->addWidget(supprimer);

    layout->addLayout(buttons);

    QHBoxLayout* search = new QHBoxLayout();

    rechercheMaintenance_ = new QLineEdit();

    rechercheMaintenance_->setPlaceholderText(
        "Rechercher une maintenance..."
    );

    QPushButton* rechercher =
        new QPushButton("Rechercher");

    QPushButton* actualiser =
        new QPushButton("Actualiser");

    search->addWidget(rechercheMaintenance_);
    search->addWidget(rechercher);
    search->addWidget(actualiser);

    layout->addLayout(search);

    maintenanceTable_ = new QTableWidget();

    maintenanceTable_->setColumnCount(7);

    maintenanceTable_->setHorizontalHeaderLabels({
        "ID",
        "Véhicule",
        "Type",
        "Date",
        "Kilométrage",
        "Coût",
        "Statut"
    });

    QStringList data = {
        "MAI-001|VEH-003|Révision moteur|20/09/2026|120 000 km|850 000 Ar|En cours",
        "MAI-002|VEH-002|Vidange|18/09/2026|62 000 km|180 000 Ar|Terminée",
        "MAI-003|VEH-001|Freins|15/09/2026|84 000 km|450 000 Ar|Terminée",
        "MAI-004|VEH-004|Pneus|12/09/2026|74 000 km|900 000 Ar|Terminée",
        "MAI-005|VEH-005|Révision générale|10/09/2026|50 000 km|700 000 Ar|Planifiée"
    };

    maintenanceTable_->setRowCount(data.size());

    for (int r = 0; r < data.size(); ++r)
    {
        QStringList values = data[r].split("|");

        for (int c = 0; c < values.size(); ++c)
        {
            maintenanceTable_->setItem(
                r,
                c,
                new QTableWidgetItem(values[c])
            );
        }
    }

    configurerTable(maintenanceTable_);

    layout->addWidget(maintenanceTable_);

    connect(ajouter, &QPushButton::clicked,
            this, [this]() { ajouterMaintenance(); });

    connect(modifier, &QPushButton::clicked,
            this, [this]() { modifierMaintenance(); });

    connect(supprimer, &QPushButton::clicked,
            this, [this]() { supprimerMaintenance(); });

    connect(rechercher, &QPushButton::clicked,
            this, [this]() { rechercherMaintenance(); });

    connect(actualiser, &QPushButton::clicked,
            this, [this]() { actualiserMaintenances(); });

    return page;
}


// ================================================================
// PAGE STATISTIQUES
// ================================================================

QWidget* MainWindow::createStatistiquesPage()
{
    QWidget* page = new QWidget();

    QScrollArea* scrollArea = new QScrollArea();

    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    QWidget* content = new QWidget();

    QVBoxLayout* layout =
        new QVBoxLayout(content);

    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(25);

    QLabel* title =
        new QLabel("Statistiques et performances");

    title->setObjectName("sectionTitle");

    layout->addWidget(title);

    // ------------------------------------------------------------
    // Cartes principales
    // ------------------------------------------------------------

    QHBoxLayout* cards =
        new QHBoxLayout();

    auto createStatCard =
        [](const QString& title,
           const QString& value,
           const QString& description)
    {
        QFrame* card = new QFrame();

        card->setObjectName("statCard");

        QVBoxLayout* cardLayout =
            new QVBoxLayout(card);

        QLabel* titleLabel =
            new QLabel(title);

        titleLabel->setObjectName("cardTitle");

        QLabel* valueLabel =
            new QLabel(value);

        valueLabel->setObjectName("cardValue");

        QLabel* descriptionLabel =
            new QLabel(description);

        descriptionLabel->setObjectName("cardDescription");

        cardLayout->addWidget(titleLabel);
        cardLayout->addWidget(valueLabel);
        cardLayout->addWidget(descriptionLabel);
        cardLayout->addStretch();

        return card;
    };

    cards->addWidget(
        createStatCard(
            "Taux de livraison",
            "94 %",
            "Livraisons réussies"
        )
    );

    cards->addWidget(
        createStatCard(
            "Distance parcourue",
            "12 450 km",
            "Ce mois"
        )
    );

    cards->addWidget(
        createStatCard(
            "Commandes",
            "248",
            "Ce mois"
        )
    );

    cards->addWidget(
        createStatCard(
            "Incidents",
            "7",
            "Ce mois"
        )
    );

    layout->addLayout(cards);

    // ------------------------------------------------------------
    // Résumé des performances
    // ------------------------------------------------------------

    QLabel* performanceTitle =
        new QLabel("Résumé des performances");

    performanceTitle->setObjectName("sectionTitle");

    layout->addWidget(performanceTitle);

    QTableWidget* statisticsTable =
        new QTableWidget();

    statisticsTable->setObjectName("dashboardTable");

    statisticsTable->setColumnCount(4);

    statisticsTable->setHorizontalHeaderLabels({
        "Indicateur",
        "Valeur actuelle",
        "Objectif",
        "État"
    });

    QStringList data = {
        "Livraisons à temps|94 %|95 %|Presque atteint",
        "Véhicules actifs|32|35|En progression",
        "Incidents résolus|86 %|90 %|En progression",
        "Utilisation flotte|78 %|80 %|Presque atteint",
        "Satisfaction clients|91 %|90 %|Objectif atteint"
    };

    statisticsTable->setRowCount(data.size());

    for (int r = 0; r < data.size(); ++r)
    {
        QStringList values = data[r].split("|");

        for (int c = 0; c < values.size(); ++c)
        {
            statisticsTable->setItem(
                r,
                c,
                new QTableWidgetItem(values[c])
            );
        }
    }

    configurerTable(statisticsTable);

    statisticsTable->setMinimumHeight(300);

    layout->addWidget(statisticsTable);

    // ------------------------------------------------------------
    // Informations supplémentaires
    // ------------------------------------------------------------

    QLabel* info =
        new QLabel(
            "Les statistiques affichées sont actuellement "
            "des données de démonstration. Elles seront "
            "alimentées par PostgreSQL lors de l'intégration "
            "de la base de données."
        );

    info->setObjectName("placeholderText");

    info->setWordWrap(true);

    layout->addWidget(info);

    layout->addStretch();

    scrollArea->setWidget(content);

    QVBoxLayout* pageLayout =
        new QVBoxLayout(page);

    pageLayout->setContentsMargins(0, 0, 0, 0);

    pageLayout->addWidget(scrollArea);

    return page;
}


// ================================================================
// BOUTON DU MENU
// ================================================================

QPushButton* MainWindow::createMenuButton(
    const QString& text)
{
    QPushButton* button =
        new QPushButton(text);

    button->setObjectName("menuButton");

    button->setProperty("selected", false);

    button->setMinimumHeight(40);

    return button;
}


// ================================================================
// NAVIGATION ENTRE LES PAGES
// ================================================================

void MainWindow::changePage(int index)
{
    pages_->setCurrentIndex(index);

    QList<QPushButton*> buttons = {
        dashboardButton_,
        commandesButton_,
        colisButton_,
        clientsButton_,
        vehiculesButton_,
        chauffeursButton_,
        entrepotsButton_,
        livraisonsButton_,
        itinerairesButton_,
        incidentsButton_,
        maintenanceButton_,
        statistiquesButton_
    };

    QStringList titles = {
        "Tableau de bord",
        "Commandes",
        "Colis",
        "Clients",
        "Véhicules",
        "Chauffeurs",
        "Entrepôts",
        "Livraisons",
        "Itinéraires",
        "Incidents",
        "Maintenance",
        "Statistiques"
    };

    QStringList subtitles = {
        "Vue générale de votre activité logistique",
        "Gestion et suivi des commandes",
        "Gestion et suivi des colis",
        "Gestion des clients",
        "Gestion de la flotte de véhicules",
        "Gestion des chauffeurs",
        "Gestion des entrepôts",
        "Suivi des livraisons",
        "Optimisation des itinéraires",
        "Gestion des incidents",
        "Suivi de la maintenance",
        "Analyse des performances"
    };

    for (int i = 0; i < buttons.size(); ++i)
    {
        bool selected = (i == index);

        buttons[i]->setProperty(
            "selected",
            selected
        );

        buttons[i]->style()->unpolish(
            buttons[i]
        );

        buttons[i]->style()->polish(
            buttons[i]
        );
    }

    if (index >= 0 &&
        index < titles.size())
    {
        pageTitle_->setText(
            titles[index]
        );

        pageSubtitle_->setText(
            subtitles[index]
        );
    }
}


// ================================================================
// FONCTION GÉNÉRIQUE DE RECHERCHE
// ================================================================

static void rechercherDansTable(
    QTableWidget* table,
    const QString& recherche)
{
    for (int row = 0;
         row < table->rowCount();
         ++row)
    {
        bool found = false;

        for (int column = 0;
             column < table->columnCount();
             ++column)
        {
            QTableWidgetItem* item =
                table->item(row, column);

            if (item &&
                item->text().contains(
                    recherche,
                    Qt::CaseInsensitive
                ))
            {
                found = true;
                break;
            }
        }

        table->setRowHidden(
            row,
            !found && !recherche.isEmpty()
        );
    }
}


// ================================================================
// COMMANDES - ACTIONS
// ================================================================

void MainWindow::ajouterCommande()
{
    int row =
        commandesTable_->rowCount();

    commandesTable_->insertRow(row);

    QString id =
        QString("#CMD-%1")
            .arg(row + 1, 3, 10, QChar('0'));

    QStringList values = {
        id,
        "Nouveau client",
        "Antananarivo",
        "Nouvelle",
        "23/09/2026",
        "0 Ar"
    };

    for (int c = 0; c < values.size(); ++c)
    {
        commandesTable_->setItem(
            row,
            c,
            new QTableWidgetItem(values[c])
        );
    }

    commandesTable_->selectRow(row);
}


void MainWindow::modifierCommande()
{
    int row =
        commandesTable_->currentRow();

    if (row < 0)
    {
        QMessageBox::information(
            this,
            "Modification",
            "Sélectionnez d'abord une commande."
        );

        return;
    }

    commandesTable_->editItem(
        commandesTable_->item(row, 1)
    );
}


void MainWindow::supprimerCommande()
{
    int row =
        commandesTable_->currentRow();

    if (row < 0)
    {
        QMessageBox::information(
            this,
            "Suppression",
            "Sélectionnez d'abord une commande."
        );

        return;
    }

    QMessageBox::StandardButton result =
        QMessageBox::question(
            this,
            "Suppression",
            "Voulez-vous supprimer cette commande ?",
            QMessageBox::Yes | QMessageBox::No
        );

    if (result == QMessageBox::Yes)
        commandesTable_->removeRow(row);
}


void MainWindow::rechercherCommande()
{
    rechercherDansTable(
        commandesTable_,
        rechercheCommande_->text().trimmed()
    );
}


void MainWindow::actualiserCommandes()
{
    rechercheCommande_->clear();

    for (int r = 0;
         r < commandesTable_->rowCount();
         ++r)
    {
        commandesTable_->setRowHidden(r, false);
    }

    commandesTable_->clearSelection();
}


// ================================================================
// COLIS - ACTIONS
// ================================================================

void MainWindow::ajouterColis()
{
    int row =
        colisTable_->rowCount();

    colisTable_->insertRow(row);

    QString id =
        QString("COL-%1")
            .arg(row + 1, 3, 10, QChar('0'));

    QStringList values = {
        id,
        "CMD-NEW",
        "Standard",
        "0 kg",
        "Nouveau",
        "Antananarivo",
        "23/09/2026"
    };

    for (int c = 0; c < values.size(); ++c)
    {
        colisTable_->setItem(
            row,
            c,
            new QTableWidgetItem(values[c])
        );
    }

    colisTable_->selectRow(row);
}


void MainWindow::modifierColis()
{
    int row =
        colisTable_->currentRow();

    if (row < 0)
    {
        QMessageBox::information(
            this,
            "Modification",
            "Sélectionnez d'abord un colis."
        );

        return;
    }

    colisTable_->editItem(
        colisTable_->item(row, 2)
    );
}


void MainWindow::supprimerColis()
{
    int row =
        colisTable_->currentRow();

    if (row < 0)
    {
        QMessageBox::information(
            this,
            "Suppression",
            "Sélectionnez d'abord un colis."
        );

        return;
    }

    if (QMessageBox::question(
            this,
            "Suppression",
            "Voulez-vous supprimer ce colis ?",
            QMessageBox::Yes | QMessageBox::No
        ) == QMessageBox::Yes)
    {
        colisTable_->removeRow(row);
    }
}


void MainWindow::rechercherColis()
{
    rechercherDansTable(
        colisTable_,
        rechercheColis_->text().trimmed()
    );
}


void MainWindow::actualiserColis()
{
    rechercheColis_->clear();

    for (int r = 0;
         r < colisTable_->rowCount();
         ++r)
    {
        colisTable_->setRowHidden(r, false);
    }

    colisTable_->clearSelection();
}


// ================================================================
// CLIENTS - ACTIONS
// ================================================================

void MainWindow::ajouterClient()
{
    int row = clientsTable_->rowCount();

    clientsTable_->insertRow(row);

    QStringList values = {
        QString("CLI-%1").arg(
            row + 1, 3, 10, QChar('0')
        ),
        "Nouveau client",
        "034 00 000 00",
        "email@mail.com",
        "Antananarivo",
        "0"
    };

    for (int c = 0; c < values.size(); ++c)
    {
        clientsTable_->setItem(
            row,
            c,
            new QTableWidgetItem(values[c])
        );
    }

    clientsTable_->selectRow(row);
}


void MainWindow::modifierClient()
{
    int row = clientsTable_->currentRow();

    if (row < 0)
    {
        QMessageBox::information(
            this,
            "Modification",
            "Sélectionnez d'abord un client."
        );
        return;
    }

    clientsTable_->editItem(
        clientsTable_->item(row, 1)
    );
}


void MainWindow::supprimerClient()
{
    int row = clientsTable_->currentRow();

    if (row < 0)
    {
        QMessageBox::information(
            this,
            "Suppression",
            "Sélectionnez d'abord un client."
        );
        return;
    }

    if (QMessageBox::question(
            this,
            "Suppression",
            "Voulez-vous supprimer ce client ?",
            QMessageBox::Yes | QMessageBox::No
        ) == QMessageBox::Yes)
    {
        clientsTable_->removeRow(row);
    }
}


void MainWindow::rechercherClient()
{
    rechercherDansTable(
        clientsTable_,
        rechercheClient_->text().trimmed()
    );
}


void MainWindow::actualiserClients()
{
    rechercheClient_->clear();

    for (int r = 0;
         r < clientsTable_->rowCount();
         ++r)
    {
        clientsTable_->setRowHidden(r, false);
    }

    clientsTable_->clearSelection();
}


// ================================================================
// VÉHICULES - ACTIONS
// ================================================================

void MainWindow::ajouterVehicule()
{
    int row = vehiculesTable_->rowCount();

    vehiculesTable_->insertRow(row);

    QStringList values = {
        QString("VEH-%1").arg(
            row + 1, 3, 10, QChar('0')
        ),
        "0000 TAA",
        "Toyota",
        "Hiace",
        "Utilitaire",
        "Disponible",
        "0 km"
    };

    for (int c = 0; c < values.size(); ++c)
    {
        vehiculesTable_->setItem(
            row,
            c,
            new QTableWidgetItem(values[c])
        );
    }

    vehiculesTable_->selectRow(row);
}


void MainWindow::modifierVehicule()
{
    int row = vehiculesTable_->currentRow();

    if (row < 0)
    {
        QMessageBox::information(
            this,
            "Modification",
            "Sélectionnez d'abord un véhicule."
        );
        return;
    }

    vehiculesTable_->editItem(
        vehiculesTable_->item(row, 1)
    );
}


void MainWindow::supprimerVehicule()
{
    int row = vehiculesTable_->currentRow();

    if (row < 0)
    {
        QMessageBox::information(
            this,
            "Suppression",
            "Sélectionnez d'abord un véhicule."
        );
        return;
    }

    if (QMessageBox::question(
            this,
            "Suppression",
            "Voulez-vous supprimer ce véhicule ?",
            QMessageBox::Yes | QMessageBox::No
        ) == QMessageBox::Yes)
    {
        vehiculesTable_->removeRow(row);
    }
}


void MainWindow::rechercherVehicule()
{
    rechercherDansTable(
        vehiculesTable_,
        rechercheVehicule_->text().trimmed()
    );
}


void MainWindow::actualiserVehicules()
{
    rechercheVehicule_->clear();

    for (int r = 0;
         r < vehiculesTable_->rowCount();
         ++r)
    {
        vehiculesTable_->setRowHidden(r, false);
    }

    vehiculesTable_->clearSelection();
}


// ================================================================
// CHAUFFEURS - ACTIONS
// ================================================================

void MainWindow::ajouterChauffeur()
{
    int row = chauffeursTable_->rowCount();

    chauffeursTable_->insertRow(row);

    QStringList values = {
        QString("CHA-%1").arg(
            row + 1, 3, 10, QChar('0')
        ),
        "Nouveau chauffeur",
        "034 00 000 00",
        "B",
        "Aucun",
        "Disponible"
    };

    for (int c = 0; c < values.size(); ++c)
    {
        chauffeursTable_->setItem(
            row,
            c,
            new QTableWidgetItem(values[c])
        );
    }

    chauffeursTable_->selectRow(row);
}


void MainWindow::modifierChauffeur()
{
    int row = chauffeursTable_->currentRow();

    if (row < 0)
    {
        QMessageBox::information(
            this,
            "Modification",
            "Sélectionnez d'abord un chauffeur."
        );
        return;
    }

    chauffeursTable_->editItem(
        chauffeursTable_->item(row, 1)
    );
}


void MainWindow::supprimerChauffeur()
{
    int row = chauffeursTable_->currentRow();

    if (row < 0)
    {
        QMessageBox::information(
            this,
            "Suppression",
            "Sélectionnez d'abord un chauffeur."
        );
        return;
    }

    if (QMessageBox::question(
            this,
            "Suppression",
            "Voulez-vous supprimer ce chauffeur ?",
            QMessageBox::Yes | QMessageBox::No
        ) == QMessageBox::Yes)
    {
        chauffeursTable_->removeRow(row);
    }
}


void MainWindow::rechercherChauffeur()
{
    rechercherDansTable(
        chauffeursTable_,
        rechercheChauffeur_->text().trimmed()
    );
}


void MainWindow::actualiserChauffeurs()
{
    rechercheChauffeur_->clear();

    for (int r = 0;
         r < chauffeursTable_->rowCount();
         ++r)
    {
        chauffeursTable_->setRowHidden(r, false);
    }

    chauffeursTable_->clearSelection();
}


// ================================================================
// ENTREPÔTS - ACTIONS
// ================================================================

void MainWindow::ajouterEntrepot()
{
    int row = entrepotsTable_->rowCount();

    entrepotsTable_->insertRow(row);

    QStringList values = {
        QString("ENT-%1").arg(
            row + 1, 3, 10, QChar('0')
        ),
        "Nouvel entrepôt",
        "Antananarivo",
        "0 colis",
        "0 colis",
        "Disponible"
    };

    for (int c = 0; c < values.size(); ++c)
    {
        entrepotsTable_->setItem(
            row,
            c,
            new QTableWidgetItem(values[c])
        );
    }

    entrepotsTable_->selectRow(row);
}


void MainWindow::modifierEntrepot()
{
    int row = entrepotsTable_->currentRow();

    if (row < 0)
    {
        QMessageBox::information(
            this,
            "Modification",
            "Sélectionnez d'abord un entrepôt."
        );
        return;
    }

    entrepotsTable_->editItem(
        entrepotsTable_->item(row, 1)
    );
}


void MainWindow::supprimerEntrepot()
{
    int row = entrepotsTable_->currentRow();

    if (row < 0)
    {
        QMessageBox::information(
            this,
            "Suppression",
            "Sélectionnez d'abord un entrepôt."
        );
        return;
    }

    if (QMessageBox::question(
            this,
            "Suppression",
            "Voulez-vous supprimer cet entrepôt ?",
            QMessageBox::Yes | QMessageBox::No
        ) == QMessageBox::Yes)
    {
        entrepotsTable_->removeRow(row);
    }
}


void MainWindow::rechercherEntrepot()
{
    rechercherDansTable(
        entrepotsTable_,
        rechercheEntrepot_->text().trimmed()
    );
}


void MainWindow::actualiserEntrepots()
{
    rechercheEntrepot_->clear();

    for (int r = 0;
         r < entrepotsTable_->rowCount();
         ++r)
    {
        entrepotsTable_->setRowHidden(r, false);
    }

    entrepotsTable_->clearSelection();
}


// ================================================================
// LIVRAISONS - ACTIONS
// ================================================================

void MainWindow::ajouterLivraison()
{
    int row = livraisonsTable_->rowCount();

    livraisonsTable_->insertRow(row);

    QStringList values = {
        QString("LIV-%1").arg(
            row + 1, 3, 10, QChar('0')
        ),
        "CMD-NEW",
        "Nouveau client",
        "Aucun",
        "Aucun",
        "Antananarivo",
        "Planifiée"
    };

    for (int c = 0; c < values.size(); ++c)
    {
        livraisonsTable_->setItem(
            row,
            c,
            new QTableWidgetItem(values[c])
        );
    }

    livraisonsTable_->selectRow(row);
}


void MainWindow::modifierLivraison()
{
    int row = livraisonsTable_->currentRow();

    if (row < 0)
    {
        QMessageBox::information(
            this,
            "Modification",
            "Sélectionnez d'abord une livraison."
        );
        return;
    }

    livraisonsTable_->editItem(
        livraisonsTable_->item(row, 2)
    );
}


void MainWindow::supprimerLivraison()
{
    int row = livraisonsTable_->currentRow();

    if (row < 0)
    {
        QMessageBox::information(
            this,
            "Suppression",
            "Sélectionnez d'abord une livraison."
        );
        return;
    }

    if (QMessageBox::question(
            this,
            "Suppression",
            "Voulez-vous supprimer cette livraison ?",
            QMessageBox::Yes | QMessageBox::No
        ) == QMessageBox::Yes)
    {
        livraisonsTable_->removeRow(row);
    }
}


void MainWindow::rechercherLivraison()
{
    rechercherDansTable(
        livraisonsTable_,
        rechercheLivraison_->text().trimmed()
    );
}


void MainWindow::actualiserLivraisons()
{
    rechercheLivraison_->clear();

    for (int r = 0;
         r < livraisonsTable_->rowCount();
         ++r)
    {
        livraisonsTable_->setRowHidden(r, false);
    }

    livraisonsTable_->clearSelection();
}


// ================================================================
// ITINÉRAIRES - ACTIONS
// ================================================================

void MainWindow::ajouterItineraire()
{
    int row = itinerairesTable_->rowCount();

    itinerairesTable_->insertRow(row);

    QStringList values = {
        QString("ITI-%1").arg(
            row + 1, 3, 10, QChar('0')
        ),
        "Antananarivo",
        "Nouvelle destination",
        "0 km",
        "0h",
        "Planifié"
    };

    for (int c = 0; c < values.size(); ++c)
    {
        itinerairesTable_->setItem(
            row,
            c,
            new QTableWidgetItem(values[c])
        );
    }

    itinerairesTable_->selectRow(row);
}


void MainWindow::modifierItineraire()
{
    int row = itinerairesTable_->currentRow();

    if (row < 0)
    {
        QMessageBox::information(
            this,
            "Modification",
            "Sélectionnez d'abord un itinéraire."
        );
        return;
    }

    itinerairesTable_->editItem(
        itinerairesTable_->item(row, 2)
    );
}


void MainWindow::supprimerItineraire()
{
    int row = itinerairesTable_->currentRow();

    if (row < 0)
    {
        QMessageBox::information(
            this,
            "Suppression",
            "Sélectionnez d'abord un itinéraire."
        );
        return;
    }

    if (QMessageBox::question(
            this,
            "Suppression",
            "Voulez-vous supprimer cet itinéraire ?",
            QMessageBox::Yes | QMessageBox::No
        ) == QMessageBox::Yes)
    {
        itinerairesTable_->removeRow(row);
    }
}


void MainWindow::rechercherItineraire()
{
    rechercherDansTable(
        itinerairesTable_,
        rechercheItineraire_->text().trimmed()
    );
}


void MainWindow::actualiserItineraires()
{
    rechercheItineraire_->clear();

    for (int r = 0;
         r < itinerairesTable_->rowCount();
         ++r)
    {
        itinerairesTable_->setRowHidden(r, false);
    }

    itinerairesTable_->clearSelection();
}


// ================================================================
// INCIDENTS - ACTIONS
// ================================================================

void MainWindow::ajouterIncident()
{
    int row = incidentsTable_->rowCount();

    incidentsTable_->insertRow(row);

    QStringList values = {
        QString("INC-%1").arg(
            row + 1, 3, 10, QChar('0')
        ),
        "23/09/2026",
        "Autre",
        "VEH-001",
        "CHA-001",
        "Nouvel incident",
        "Ouvert"
    };

    for (int c = 0; c < values.size(); ++c)
    {
        incidentsTable_->setItem(
            row,
            c,
            new QTableWidgetItem(values[c])
        );
    }

    incidentsTable_->selectRow(row);
}


void MainWindow::modifierIncident()
{
    int row = incidentsTable_->currentRow();

    if (row < 0)
    {
        QMessageBox::information(
            this,
            "Modification",
            "Sélectionnez d'abord un incident."
        );
        return;
    }

    incidentsTable_->editItem(
        incidentsTable_->item(row, 5)
    );
}


void MainWindow::supprimerIncident()
{
    int row = incidentsTable_->currentRow();

    if (row < 0)
    {
        QMessageBox::information(
            this,
            "Suppression",
            "Sélectionnez d'abord un incident."
        );
        return;
    }

    if (QMessageBox::question(
            this,
            "Suppression",
            "Voulez-vous supprimer cet incident ?",
            QMessageBox::Yes | QMessageBox::No
        ) == QMessageBox::Yes)
    {
        incidentsTable_->removeRow(row);
    }
}


void MainWindow::rechercherIncident()
{
    rechercherDansTable(
        incidentsTable_,
        rechercheIncident_->text().trimmed()
    );
}


void MainWindow::actualiserIncidents()
{
    rechercheIncident_->clear();

    for (int r = 0;
         r < incidentsTable_->rowCount();
         ++r)
    {
        incidentsTable_->setRowHidden(r, false);
    }

    incidentsTable_->clearSelection();
}


// ================================================================
// MAINTENANCE - ACTIONS
// ================================================================

void MainWindow::ajouterMaintenance()
{
    int row = maintenanceTable_->rowCount();

    maintenanceTable_->insertRow(row);

    QStringList values = {
        QString("MAI-%1").arg(
            row + 1, 3, 10, QChar('0')
        ),
        "VEH-001",
        "Révision",
        "23/09/2026",
        "0 km",
        "0 Ar",
        "Planifiée"
    };

    for (int c = 0; c < values.size(); ++c)
    {
        maintenanceTable_->setItem(
            row,
            c,
            new QTableWidgetItem(values[c])
        );
    }

    maintenanceTable_->selectRow(row);
}


void MainWindow::modifierMaintenance()
{
    int row = maintenanceTable_->currentRow();

    if (row < 0)
    {
        QMessageBox::information(
            this,
            "Modification",
            "Sélectionnez d'abord une maintenance."
        );
        return;
    }

    maintenanceTable_->editItem(
        maintenanceTable_->item(row, 2)
    );
}


void MainWindow::supprimerMaintenance()
{
    int row = maintenanceTable_->currentRow();

    if (row < 0)
    {
        QMessageBox::information(
            this,
            "Suppression",
            "Sélectionnez d'abord une maintenance."
        );
        return;
    }

    if (QMessageBox::question(
            this,
            "Suppression",
            "Voulez-vous supprimer cette maintenance ?",
            QMessageBox::Yes | QMessageBox::No
        ) == QMessageBox::Yes)
    {
        maintenanceTable_->removeRow(row);
    }
}


void MainWindow::rechercherMaintenance()
{
    rechercherDansTable(
        maintenanceTable_,
        rechercheMaintenance_->text().trimmed()
    );
}


void MainWindow::actualiserMaintenances()
{
    rechercheMaintenance_->clear();

    for (int r = 0;
         r < maintenanceTable_->rowCount();
         ++r)
    {
        maintenanceTable_->setRowHidden(r, false);
    }

    maintenanceTable_->clearSelection();
}


// ================================================================
// STATISTIQUES - ACTUALISATION
// ================================================================

void MainWindow::actualiserStatistiques()
{
    // Les statistiques sont actuellement statiques.
    // Elles seront calculées automatiquement après
    // la connexion à PostgreSQL.
}