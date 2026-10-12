#include "MainWindow.hpp"
#include "Style.hpp"
#include "GuiDatabase.hpp"
#include "FormulaireDialog.hpp"

#include <QCloseEvent>
#include <QDate>
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
#include <QRegularExpression>
#include <QTimer>
#include <algorithm>
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

    // Connexion PostgreSQL (silencieuse : sans base, l'interface
    // fonctionne avec ses données de démonstration)
    initialiserBaseDeDonnees();
}

MainWindow::~MainWindow() = default;


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

    etatBaseLabel_ = new QLabel("Base : hors ligne");
    etatBaseLabel_->setObjectName("pageSubtitle");

    QPushButton* chargerBase = new QPushButton("Charger depuis la base");
    QPushButton* enregistrerBase = new QPushButton("Enregistrer dans la base");

    connect(chargerBase, &QPushButton::clicked,
            this, [this]() { chargerDepuisBase(false); });

    connect(enregistrerBase, &QPushButton::clicked,
            this, [this]() { sauvegarderVersBase(false); });

    layout->addWidget(etatBaseLabel_);
    layout->addSpacing(12);
    layout->addWidget(chargerBase);
    layout->addWidget(enregistrerBase);
    layout->addSpacing(20);

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

// Prochain identifiant libre ("CLI-006"...). Evite les doublons quand une
// ligne du milieu a été supprimée (row + 1 pouvait réutiliser un identifiant).
static int prochainNumero(QTableWidget* table, const QString& prefixe)
{
    int maximum = 0;

    for (int r = 0; r < table->rowCount(); ++r)
    {
        const QTableWidgetItem* item = table->item(r, 0);
        if (!item) continue;

        const QString texte = item->text();
        if (!texte.startsWith(prefixe + "-")) continue;

        maximum = qMax(maximum, texte.mid(prefixe.size() + 1).toInt());
    }
    return maximum + 1;
}

static QString prochainCode(QTableWidget* table, const QString& prefixe)
{
    return QString("%1-%2").arg(prefixe).arg(prochainNumero(table, prefixe), 3, 10, QChar('0'));
}

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
    ajouterLigne(commandesTable_, "#CMD", "Nouveau : commande");
}


void MainWindow::modifierCommande()
{
    modifierLigne(commandesTable_, "Modifier : commande");
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
    rechargerDepuisBase(5);

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
    ajouterLigne(colisTable_, "COL", "Nouveau : colis");
}


void MainWindow::modifierColis()
{
    modifierLigne(colisTable_, "Modifier : colis");
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
    rechargerDepuisBase(6);

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
    ajouterLigne(clientsTable_, "CLI", "Nouveau : client");
}


void MainWindow::modifierClient()
{
    modifierLigne(clientsTable_, "Modifier : client");
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
    rechargerDepuisBase(0);

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
    ajouterLigne(vehiculesTable_, "VEH", "Nouveau : véhicule");
}


void MainWindow::modifierVehicule()
{
    modifierLigne(vehiculesTable_, "Modifier : véhicule");
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
    rechargerDepuisBase(1);

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
    ajouterLigne(chauffeursTable_, "CHA", "Nouveau : chauffeur");
}


void MainWindow::modifierChauffeur()
{
    modifierLigne(chauffeursTable_, "Modifier : chauffeur");
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
    rechargerDepuisBase(2);

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
    ajouterLigne(entrepotsTable_, "ENT", "Nouveau : entrepôt");
}


void MainWindow::modifierEntrepot()
{
    modifierLigne(entrepotsTable_, "Modifier : entrepôt");
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
    rechargerDepuisBase(3);

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
    ajouterLigne(livraisonsTable_, "LIV", "Nouveau : livraison");
}


void MainWindow::modifierLivraison()
{
    modifierLigne(livraisonsTable_, "Modifier : livraison");
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
    rechargerDepuisBase(7);

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
    ajouterLigne(itinerairesTable_, "ITI", "Nouveau : itinéraire");
}


void MainWindow::modifierItineraire()
{
    modifierLigne(itinerairesTable_, "Modifier : itinéraire");
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
    rechargerDepuisBase(4);

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
    ajouterLigne(incidentsTable_, "INC", "Nouveau : incident");
}


void MainWindow::modifierIncident()
{
    modifierLigne(incidentsTable_, "Modifier : incident");
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
    rechargerDepuisBase(8);

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
    ajouterLigne(maintenanceTable_, "MAI", "Nouveau : intervention de maintenance");
}


void MainWindow::modifierMaintenance()
{
    modifierLigne(maintenanceTable_, "Modifier : intervention de maintenance");
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
    rechargerDepuisBase(9);

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


// ================================================================
// FORMULAIRES D'AJOUT / MODIFICATION
// ================================================================

namespace
{
    const QStringList VILLES = {
        "Antananarivo", "Toamasina", "Fianarantsoa", "Mahajanga",
        "Antsirabe", "Toliara", "Antsiranana"
    };

    QStringList valeursDeColonne(const QTableWidget* table, int colonne, bool sansDiese = false)
    {
        QStringList liste;

        for (int r = 0; r < table->rowCount(); ++r)
        {
            const QTableWidgetItem* item = table->item(r, colonne);
            if (!item) continue;

            QString valeur = item->text().trimmed();
            if (sansDiese && valeur.startsWith('#')) valeur.remove(0, 1);

            if (!valeur.isEmpty() && !liste.contains(valeur))
            {
                liste << valeur;
            }
        }
        return liste;
    }

    QString texteDe(const QTableWidget* table, int ligne, int colonne)
    {
        const QTableWidgetItem* item = table->item(ligne, colonne);
        return item ? item->text().trimmed() : QString();
    }

    double nombreDe(QString texte)
    {
        texte.remove(QRegularExpression("[^0-9,.]"));
        texte.replace(',', '.');
        return texte.toDouble();
    }

    QString enMinuscules(const QString& texte)
    {
        return texte.trimmed().toLower();
    }
}

QList<ChampFormulaire> MainWindow::champsPour(QTableWidget* table) const
{
    using T = ChampFormulaire::Type;

    auto champ = [](const QString& etiquette,
                    T type = T::Texte,
                    const QStringList& choix = {},
                    const QString& suffixe = QString(),
                    bool requis = false)
    {
        ChampFormulaire c;
        c.etiquette = etiquette;
        c.type = type;
        c.choix = choix;
        c.suffixe = suffixe;
        c.requis = requis;
        return c;
    };

    auto avecAucun = [](QStringList liste)
    {
        liste.prepend("Aucun");
        return liste;
    };

    if (table == clientsTable_)
    {
        return {
            champ("ID", T::LectureSeule),
            champ("Nom", T::Texte, {}, {}, true),
            champ("Téléphone"),
            champ("Email"),
            champ("Ville", T::ChoixEditable, VILLES),
            champ("Commandes", T::LectureSeule)
        };
    }

    if (table == vehiculesTable_)
    {
        return {
            champ("ID", T::LectureSeule),
            champ("Immatriculation", T::Texte, {}, {}, true),
            champ("Marque"),
            champ("Modèle"),
            champ("Type", T::ChoixEditable,
                  {"Utilitaire", "Fourgon", "Camion", "Moto", "Frigorifique"}),
            champ("Statut", T::Choix,
                  {"Disponible", "Actif", "En mission", "Maintenance", "Indisponible"}),
            champ("Kilométrage", T::Nombre, {}, " km")
        };
    }

    if (table == chauffeursTable_)
    {
        return {
            champ("ID", T::LectureSeule),
            champ("Nom", T::Texte, {}, {}, true),
            champ("Téléphone"),
            champ("Permis", T::Choix, {"B", "C", "C1", "MOTO"}),
            champ("Véhicule", T::Choix, avecAucun(valeursDeColonne(vehiculesTable_, 0))),
            champ("Statut", T::Choix,
                  {"Disponible", "En livraison", "En congé", "Indisponible"})
        };
    }

    if (table == entrepotsTable_)
    {
        return {
            champ("ID", T::LectureSeule),
            champ("Nom", T::Texte, {}, {}, true),
            champ("Ville", T::ChoixEditable, VILLES),
            champ("Capacité", T::Nombre, {}, " colis"),
            champ("Occupation", T::Nombre, {}, " colis"),
            champ("Statut", T::Choix,
                  {"Actif", "Disponible", "Presque plein", "Plein", "Inactif"})
        };
    }

    if (table == itinerairesTable_)
    {
        return {
            champ("ID", T::LectureSeule),
            champ("Départ", T::ChoixEditable, VILLES, {}, true),
            champ("Destination", T::ChoixEditable, VILLES, {}, true),
            champ("Distance", T::Nombre, {}, " km"),
            champ("Durée (ex : 8h 30)"),
            champ("Statut", T::Choix, {"Planifié", "Actif", "Optimisé", "Terminé"})
        };
    }

    if (table == commandesTable_)
    {
        return {
            champ("Commande", T::LectureSeule),
            champ("Client", T::Choix, valeursDeColonne(clientsTable_, 1), {}, true),
            champ("Destination", T::ChoixEditable, VILLES),
            champ("Statut", T::Choix,
                  {"En attente", "En cours", "En transit", "Livrée", "Incident", "Annulée"}),
            champ("Date", T::Date),
            champ("Montant", T::Nombre, {}, " Ar")
        };
    }

    if (table == colisTable_)
    {
        return {
            champ("ID Colis", T::LectureSeule),
            champ("Commande", T::Choix, valeursDeColonne(commandesTable_, 0, true), {}, true),
            champ("Type", T::ChoixEditable, {"Standard", "Fragile", "Volumineux"}),
            champ("Poids", T::Nombre, {}, " kg"),
            champ("Statut", T::Choix,
                  {"En attente", "En cours", "En transit", "Livré", "Incident"}),
            champ("Destination", T::ChoixEditable, VILLES),
            champ("Date", T::Date)
        };
    }

    if (table == livraisonsTable_)
    {
        return {
            champ("ID", T::LectureSeule),
            champ("Commande", T::Choix, valeursDeColonne(commandesTable_, 0, true), {}, true),
            champ("Client (selon la commande)", T::LectureSeule),
            champ("Chauffeur", T::Choix, avecAucun(valeursDeColonne(chauffeursTable_, 0))),
            champ("Véhicule", T::Choix, avecAucun(valeursDeColonne(vehiculesTable_, 0))),
            champ("Destination", T::ChoixEditable, VILLES),
            champ("Statut", T::Choix,
                  {"Planifiée", "En cours", "En transit", "Livrée", "Incident", "Annulée"})
        };
    }

    if (table == incidentsTable_)
    {
        return {
            champ("ID", T::LectureSeule),
            champ("Date", T::Date),
            champ("Type", T::ChoixEditable, {"Panne", "Accident", "Retard", "Autre"}, {}, true),
            champ("Véhicule", T::Choix, avecAucun(valeursDeColonne(vehiculesTable_, 0))),
            champ("Chauffeur", T::Choix, avecAucun(valeursDeColonne(chauffeursTable_, 0))),
            champ("Description", T::Texte, {}, {}, true),
            champ("Statut", T::Choix, {"Ouvert", "Traité", "Clos"})
        };
    }

    if (table == maintenanceTable_)
    {
        return {
            champ("ID", T::LectureSeule),
            champ("Véhicule", T::Choix, valeursDeColonne(vehiculesTable_, 0), {}, true),
            champ("Type", T::ChoixEditable,
                  {"Révision moteur", "Vidange", "Freins", "Pneus", "Révision générale"},
                  {}, true),
            champ("Date", T::Date),
            champ("Kilométrage", T::Nombre, {}, " km"),
            champ("Coût", T::Nombre, {}, " Ar"),
            champ("Statut", T::Choix, {"Planifiée", "En cours", "Terminée"})
        };
    }

    return {};
}

// Valeurs qui dépendent d'autres tableaux (client d'une livraison, nombre
// de commandes d'un client).
void MainWindow::completerValeursDerivees(QTableWidget* table, QStringList& valeurs) const
{
    if (table == livraisonsTable_ && valeurs.size() > 2)
    {
        QString client;
        for (int r = 0; r < commandesTable_->rowCount(); ++r)
        {
            QString code = texteDe(commandesTable_, r, 0);
            if (code.startsWith('#')) code.remove(0, 1);

            if (code == valeurs[1])
            {
                client = texteDe(commandesTable_, r, 1);
                break;
            }
        }
        valeurs[2] = client;
    }

    if (table == clientsTable_ && valeurs.size() > 5)
    {
        int nombre = 0;
        for (int r = 0; r < commandesTable_->rowCount(); ++r)
        {
            if (texteDe(commandesTable_, r, 1) == valeurs[1]) ++nombre;
        }
        valeurs[5] = QString::number(nombre);
    }
}

void MainWindow::ajouterLigne(QTableWidget* table, const QString& prefixe, const QString& titre)
{
    using T = ChampFormulaire::Type;

    const QList<ChampFormulaire> champs = champsPour(table);
    if (champs.isEmpty()) return;

    // Valeurs de départ : première option des listes, date du jour, 0 pour les nombres
    QStringList initiales;
    for (const ChampFormulaire& champ : champs)
    {
        switch (champ.type)
        {
            case T::Choix:
            case T::ChoixEditable:
                initiales << (champ.choix.isEmpty() ? QString() : champ.choix.first());
                break;
            case T::Date:
                initiales << QDate::currentDate().toString("dd/MM/yyyy");
                break;
            case T::Nombre:
                initiales << ("0" + champ.suffixe);
                break;
            default:
                initiales << QString();
                break;
        }
    }

    initiales[0] = prochainCode(table, prefixe);
    if (table == clientsTable_) initiales[5] = "0";

    FormulaireDialog dialogue(titre, champs, initiales, this);
    if (dialogue.exec() != QDialog::Accepted)
    {
        return;
    }

    QStringList valeurs = dialogue.valeurs();
    valeurs[0] = initiales[0];
    completerValeursDerivees(table, valeurs);

    const int ligne = table->rowCount();
    table->insertRow(ligne);

    for (int c = 0; c < valeurs.size() && c < table->columnCount(); ++c)
    {
        table->setItem(ligne, c, new QTableWidgetItem(valeurs[c]));
    }

    table->selectRow(ligne);
    table->scrollToItem(table->item(ligne, 0));
}

void MainWindow::modifierLigne(QTableWidget* table, const QString& titre)
{
    const int ligne = table->currentRow();

    if (ligne < 0)
    {
        QMessageBox::information(
            this,
            "Modification",
            "Sélectionnez d'abord une ligne dans le tableau."
        );
        return;
    }

    const QList<ChampFormulaire> champs = champsPour(table);
    if (champs.isEmpty()) return;

    QStringList initiales;
    for (int c = 0; c < champs.size(); ++c)
    {
        initiales << texteDe(table, ligne, c);
    }

    FormulaireDialog dialogue(titre, champs, initiales, this);
    if (dialogue.exec() != QDialog::Accepted)
    {
        return;
    }

    QStringList valeurs = dialogue.valeurs();
    valeurs[0] = initiales[0];   // l'identifiant ne change jamais
    completerValeursDerivees(table, valeurs);

    // Un client renommé : ses commandes suivent (elles le désignent par son nom)
    if (table == clientsTable_ && initiales.size() > 1 && valeurs[1] != initiales[1])
    {
        for (int r = 0; r < commandesTable_->rowCount(); ++r)
        {
            if (texteDe(commandesTable_, r, 1) == initiales[1])
            {
                commandesTable_->item(r, 1)->setText(valeurs[1]);
            }
        }
    }

    for (int c = 0; c < valeurs.size() && c < table->columnCount(); ++c)
    {
        QTableWidgetItem* item = table->item(ligne, c);
        if (item)
        {
            item->setText(valeurs[c]);
        }
        else
        {
            table->setItem(ligne, c, new QTableWidgetItem(valeurs[c]));
        }
    }
}


// ================================================================
// BASE DE DONNÉES POSTGRESQL
// ================================================================

namespace
{
    const char* libelleTable(int identifiant)
    {
        static const char* const libelles[NOMBRE_TABLES] = {
            "clients", "véhicules", "chauffeurs", "entrepôts", "itinéraires",
            "commandes", "colis", "livraisons", "incidents", "maintenance"
        };
        return libelles[identifiant];
    }
}

QTableWidget* MainWindow::tablePour(int identifiant) const
{
    switch (static_cast<TableId>(identifiant))
    {
        case TableId::Clients:     return clientsTable_;
        case TableId::Vehicules:   return vehiculesTable_;
        case TableId::Chauffeurs:  return chauffeursTable_;
        case TableId::Entrepots:   return entrepotsTable_;
        case TableId::Itineraires: return itinerairesTable_;
        case TableId::Commandes:   return commandesTable_;
        case TableId::Colis:       return colisTable_;
        case TableId::Livraisons:  return livraisonsTable_;
        case TableId::Incidents:   return incidentsTable_;
        case TableId::Maintenance: return maintenanceTable_;
    }
    return nullptr;
}

void MainWindow::initialiserBaseDeDonnees()
{
    baseDeDonnees_ = std::make_unique<GuiDatabase>();

    // Enregistrement automatique 0,7 s après la dernière modification
    sauvegardeTimer_ = new QTimer(this);
    sauvegardeTimer_->setSingleShot(true);
    sauvegardeTimer_->setInterval(700);

    connect(sauvegardeTimer_, &QTimer::timeout,
            this, [this]() { sauvegarderVersBase(true); });

    // Tableau de bord et statistiques recalculés peu après chaque changement
    statistiquesTimer_ = new QTimer(this);
    statistiquesTimer_->setSingleShot(true);
    statistiquesTimer_->setInterval(150);

    connect(statistiquesTimer_, &QTimer::timeout,
            this, [this]() { actualiserTableauDeBord(); });

    for (int i = 0; i < NOMBRE_TABLES; ++i)
    {
        surveillerTable(tablePour(i), i);
    }

    chargerDepuisBase(true);
    actualiserTableauDeBord();
}

void MainWindow::surveillerTable(QTableWidget* table, int identifiant)
{
    auto declencheur = [this, identifiant]()
    {
        planifierSauvegarde(identifiant);
        statistiquesTimer_->start();
    };

    connect(table->model(), &QAbstractItemModel::rowsInserted, this, declencheur);
    connect(table->model(), &QAbstractItemModel::rowsRemoved,  this, declencheur);
    connect(table->model(), &QAbstractItemModel::dataChanged,  this, declencheur);
}

void MainWindow::planifierSauvegarde(int identifiant)
{
    if (synchronisationEnCours_ || !autoSauvegarde_)
    {
        return;
    }

    tablesModifiees_ |= (1u << identifiant);
    sauvegardeTimer_->start();
}

void MainWindow::afficherEtatBase(const QString& texte, bool erreur)
{
    etatBaseLabel_->setText(texte);
    etatBaseLabel_->setStyleSheet(
        erreur ? "color: #dc2626; font-weight: 600;"
               : "color: #16a34a; font-weight: 600;");
}

void MainWindow::chargerDepuisBase(bool silencieux)
{
    QStringList erreurs;
    QStringList details;
    bool vide[NOMBRE_TABLES] = {};
    int totalLignes = 0;

    synchronisationEnCours_ = true;

    for (int i = 0; i < NOMBRE_TABLES; ++i)
    {
        const ResultatSync resultat =
            baseDeDonnees_->charger(static_cast<TableId>(i), tablePour(i), false);

        if (!resultat.ok)
        {
            erreurs << resultat.erreurs;
            break;   // inutile d'attendre 10 fois un serveur injoignable
        }

        vide[i] = (resultat.lignes == 0);
        totalLignes += resultat.lignes;
        details << QString("%1 : %2 ligne(s)").arg(libelleTable(i)).arg(resultat.lignes);
    }

    const bool baseVide = erreurs.isEmpty() && totalLignes == 0;

    // La base fait foi : une table vide en base est vide à l'écran.
    // Exception : base entièrement vide (première utilisation), où les
    // données de démonstration servent à l'alimenter.
    if (erreurs.isEmpty() && !baseVide)
    {
        for (int i = 0; i < NOMBRE_TABLES; ++i)
        {
            if (vide[i])
            {
                tablePour(i)->setRowCount(0);
            }
        }
    }

    synchronisationEnCours_ = false;

    if (!erreurs.isEmpty())
    {
        autoSauvegarde_ = false;
        afficherEtatBase("Base : hors ligne", true);
        etatBaseLabel_->setToolTip(erreurs.join("\n"));

        if (!silencieux)
        {
            QMessageBox::warning(
                this,
                "PostgreSQL",
                "Connexion impossible :\n" + erreurs.join("\n") +
                "\n\nVérifiez que le serveur tourne et que la variable "
                "d'environnement SL_DB_PASSWORD est définie.");
        }
        actualiserTableauDeBord();
        return;
    }

    autoSauvegarde_ = true;
    afficherEtatBase("Base : PostgreSQL connecté", false);
    etatBaseLabel_->setToolTip(details.join("\n"));

    if (baseVide)
    {
        // Première utilisation : les données de démonstration alimentent la base
        tablesModifiees_ = (1u << NOMBRE_TABLES) - 1;
        sauvegarderVersBase(true);
        details << "Base vide : données de démonstration enregistrées.";
    }

    actualiserTableauDeBord();

    if (!silencieux)
    {
        QMessageBox::information(this, "PostgreSQL", details.join("\n"));
    }
}

void MainWindow::rechargerDepuisBase(int identifiant)
{
    if (!baseDeDonnees_)
    {
        return;
    }

    // Ne jamais écraser une modification pas encore enregistrée
    if (sauvegardeTimer_ && sauvegardeTimer_->isActive())
    {
        sauvegardeTimer_->stop();
        sauvegarderVersBase(true);
    }

    synchronisationEnCours_ = true;
    const ResultatSync resultat =
        baseDeDonnees_->charger(static_cast<TableId>(identifiant), tablePour(identifiant), true);
    synchronisationEnCours_ = false;

    if (resultat.ok)
    {
        autoSauvegarde_ = true;
        afficherEtatBase("Base : PostgreSQL connecté", false);
        etatBaseLabel_->setToolTip(QString());
    }
    else
    {
        afficherEtatBase("Base : hors ligne", true);
        etatBaseLabel_->setToolTip(resultat.erreurs.join("\n"));
    }

    actualiserTableauDeBord();
}

void MainWindow::sauvegarderVersBase(bool silencieux)
{
    // Enregistrement automatique : seulement les tables modifiées.
    // Bouton manuel : toutes les tables.
    const unsigned masque = silencieux ? tablesModifiees_ : ~0u;
    tablesModifiees_ = 0;

    QStringList erreurs;
    bool connexionPerdue = false;
    unsigned aRecharger = 0;
    int total = 0;

    // L'ordre de TableId respecte les clés étrangères (clients avant commandes...)
    for (int i = 0; i < NOMBRE_TABLES; ++i)
    {
        if ((masque & (1u << i)) == 0)
        {
            continue;
        }

        const ResultatSync resultat =
            baseDeDonnees_->sauvegarder(static_cast<TableId>(i), tablePour(i));
        total += resultat.lignes;

        for (const QString& erreur : resultat.erreurs)
        {
            erreurs << QString("%1 — %2").arg(libelleTable(i), erreur);
        }

        if (resultat.suppressionRefusee)
        {
            aRecharger |= (1u << i);
        }

        if (!resultat.ok)
        {
            connexionPerdue = true;
            break;
        }
    }

    // Une suppression refusée par la base (élément encore utilisé) : la ligne
    // réapparaît à l'écran au lieu de rester « supprimée » à tort.
    if (aRecharger != 0)
    {
        synchronisationEnCours_ = true;
        for (int i = 0; i < NOMBRE_TABLES; ++i)
        {
            if (aRecharger & (1u << i))
            {
                baseDeDonnees_->charger(static_cast<TableId>(i), tablePour(i), true);
            }
        }
        synchronisationEnCours_ = false;
    }

    if (connexionPerdue)
    {
        autoSauvegarde_ = false;
        afficherEtatBase("Base : hors ligne", true);
        etatBaseLabel_->setToolTip(erreurs.join("\n"));
    }
    else
    {
        autoSauvegarde_ = true;
        afficherEtatBase(
            erreurs.isEmpty() ? "Base : enregistré"
                              : "Base : enregistré avec erreurs",
            !erreurs.isEmpty());
        etatBaseLabel_->setToolTip(erreurs.join("\n"));
    }

    if (!erreurs.isEmpty())
    {
        QMessageBox::warning(this, "PostgreSQL", erreurs.join("\n"));
    }
    else if (!silencieux)
    {
        QMessageBox::information(
            this,
            "PostgreSQL",
            QString("%1 ligne(s) enregistrée(s) dans la base.").arg(total));
    }

    actualiserTableauDeBord();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    // Enregistre les dernières modifications avant de quitter
    if (sauvegardeTimer_ && sauvegardeTimer_->isActive() && autoSauvegarde_)
    {
        sauvegardeTimer_->stop();
        sauvegarderVersBase(true);
    }

    QMainWindow::closeEvent(event);
}


// ================================================================
// TABLEAU DE BORD ET STATISTIQUES (calculés depuis les tableaux,
// donc toujours identiques à la base de données)
// ================================================================

void MainWindow::actualiserTableauDeBord()
{
    if (!pages_ || !commandesTable_ || !livraisonsTable_ ||
        !incidentsTable_ || !vehiculesTable_ || !entrepotsTable_)
    {
        return;
    }

    auto compter = [](const QTableWidget* table, int colonne, const QStringList& statuts)
    {
        int nombre = 0;
        for (int r = 0; r < table->rowCount(); ++r)
        {
            if (statuts.contains(enMinuscules(texteDe(table, r, colonne))))
            {
                ++nombre;
            }
        }
        return nombre;
    };

    auto pourcentage = [](double partie, double total)
    {
        return total > 0 ? partie * 100.0 / total : 0.0;
    };

    auto etat = [](double valeur, double objectif)
    {
        if (valeur >= objectif)        return QString("Objectif atteint");
        if (valeur >= objectif * 0.9)  return QString("Presque atteint");
        return QString("En progression");
    };

    auto groupe = [](long long valeur)
    {
        QString chiffres = QString::number(valeur);
        QString resultat;
        int n = 0;
        for (int i = chiffres.size() - 1; i >= 0; --i)
        {
            if (n > 0 && n % 3 == 0) resultat.prepend(' ');
            resultat.prepend(chiffres[i]);
            ++n;
        }
        return resultat;
    };

    const int nbCommandes   = commandesTable_->rowCount();
    const int nbLivraisons  = livraisonsTable_->rowCount();
    const int nbIncidents   = incidentsTable_->rowCount();
    const int nbVehicules   = vehiculesTable_->rowCount();

    const int livrees       = compter(livraisonsTable_, 6, {"livrée", "livree", "livré"});
    const int enCours       = compter(livraisonsTable_, 6, {"en cours", "en transit"});
    const int ouverts       = compter(incidentsTable_, 6, {"ouvert"});
    const int resolus       = compter(incidentsTable_, 6, {"traité", "traite", "clos"});
    const int horsService   = compter(vehiculesTable_, 5, {"maintenance", "indisponible", "en maintenance"});
    const int enActivite    = compter(vehiculesTable_, 5, {"actif", "en mission", "en service"});

    double kmTotal = 0;
    for (int r = 0; r < vehiculesTable_->rowCount(); ++r)
    {
        kmTotal += nombreDe(texteDe(vehiculesTable_, r, 6));
    }

    double stock = 0, capacite = 0;
    for (int r = 0; r < entrepotsTable_->rowCount(); ++r)
    {
        capacite += nombreDe(texteDe(entrepotsTable_, r, 3));
        stock    += nombreDe(texteDe(entrepotsTable_, r, 4));
    }

    const double tauxLivraison = pourcentage(livrees, nbLivraisons);
    const double tauxIncidents = pourcentage(resolus, nbIncidents);
    const double tauxFlotte    = pourcentage(enActivite, nbVehicules);
    const double tauxEntrepots = pourcentage(stock, capacite);

    // ---------------- Tableau de bord ----------------

    QWidget* tableauDeBord = pages_->widget(0);

    const QList<QLabel*> valeurs =
        tableauDeBord->findChildren<QLabel*>("cardValue");

    if (valeurs.size() >= 4)
    {
        valeurs[0]->setText(QString::number(nbCommandes));
        valeurs[1]->setText(QString::number(livrees));
        valeurs[2]->setText(QString::number(enCours));
        valeurs[3]->setText(QString::number(ouverts));
    }

    // Deuxième rangée : état de la flotte
    if (valeurs.size() >= 7)
    {
        valeurs[4]->setText(QString::number(enActivite));
        valeurs[5]->setText(QString::number(compter(vehiculesTable_, 5, {"maintenance", "en maintenance"})));
        valeurs[6]->setText(QString::number(compter(vehiculesTable_, 5, {"disponible"})));
    }

    if (QTableWidget* recentes = tableauDeBord->findChild<QTableWidget*>("dashboardTable"))
    {
        // Les 5 commandes les plus récentes
        QList<int> lignes;
        for (int r = 0; r < commandesTable_->rowCount(); ++r) lignes << r;

        std::stable_sort(lignes.begin(), lignes.end(), [this](int a, int b)
        {
            const QDate da = QDate::fromString(texteDe(commandesTable_, a, 4), "dd/MM/yyyy");
            const QDate db = QDate::fromString(texteDe(commandesTable_, b, 4), "dd/MM/yyyy");
            return da > db;
        });

        const int affichees = qMin(5, lignes.size());
        recentes->setRowCount(affichees);

        for (int i = 0; i < affichees; ++i)
        {
            for (int c = 0; c < 5; ++c)
            {
                recentes->setItem(i, c,
                    new QTableWidgetItem(texteDe(commandesTable_, lignes[i], c)));
            }
        }
    }

    // ---------------- Statistiques ----------------

    QWidget* statistiques = pages_->widget(11);

    const QList<QLabel*> valeursStat =
        statistiques->findChildren<QLabel*>("cardValue");
    const QList<QLabel*> descriptionsStat =
        statistiques->findChildren<QLabel*>("cardDescription");

    if (valeursStat.size() >= 4 && descriptionsStat.size() >= 4)
    {
        valeursStat[0]->setText(QString("%1 %").arg(qRound(tauxLivraison)));
        descriptionsStat[0]->setText("Livraisons réussies");

        valeursStat[1]->setText(groupe(qRound64(kmTotal)) + " km");
        descriptionsStat[1]->setText("Kilométrage total de la flotte");

        valeursStat[2]->setText(QString::number(nbCommandes));
        descriptionsStat[2]->setText("Commandes enregistrées");

        valeursStat[3]->setText(QString::number(nbIncidents));
        descriptionsStat[3]->setText("Incidents enregistrés");
    }

    if (QTableWidget* tableStat = statistiques->findChild<QTableWidget*>("dashboardTable"))
    {
        struct Ligne { QString indicateur, valeur, objectif, etat; };

        const QList<Ligne> lignes = {
            {"Livraisons réussies",
             QString("%1 %").arg(qRound(tauxLivraison)), "95 %", etat(tauxLivraison, 95)},

            {"Véhicules disponibles",
             QString::number(nbVehicules - horsService), QString::number(nbVehicules),
             etat(nbVehicules - horsService, nbVehicules)},

            {"Incidents résolus",
             QString("%1 %").arg(qRound(tauxIncidents)), "90 %", etat(tauxIncidents, 90)},

            {"Utilisation flotte",
             QString("%1 %").arg(qRound(tauxFlotte)), "80 %", etat(tauxFlotte, 80)},

            {"Occupation entrepôts",
             QString("%1 %").arg(qRound(tauxEntrepots)), "80 %", etat(tauxEntrepots, 80)}
        };

        tableStat->setRowCount(lignes.size());
        for (int r = 0; r < lignes.size(); ++r)
        {
            tableStat->setItem(r, 0, new QTableWidgetItem(lignes[r].indicateur));
            tableStat->setItem(r, 1, new QTableWidgetItem(lignes[r].valeur));
            tableStat->setItem(r, 2, new QTableWidgetItem(lignes[r].objectif));
            tableStat->setItem(r, 3, new QTableWidgetItem(lignes[r].etat));
        }
    }

    if (QLabel* info = statistiques->findChild<QLabel*>("placeholderText"))
    {
        info->setText(
            autoSauvegarde_
                ? "Statistiques calculées à partir des données de l'application, "
                  "synchronisées avec PostgreSQL."
                : "Mode hors ligne : statistiques calculées à partir des données "
                  "de démonstration affichées.");
    }
}