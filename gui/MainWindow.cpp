#include "MainWindow.hpp"
#include "Style.hpp"
#include "GuiDatabase.hpp"
#include "FormulaireDialog.hpp"
#include "entities/Chauffeur.hpp"
#include "entities/Commande.hpp"
#include "factories/ColisFactory.hpp"
#include "factories/VehiculeFactory.hpp"
#include "graphe/Dijkstra.hpp"
#include "graphe/ReseauRoutier.hpp"
#include "services/AllocateurRessources.hpp"
#include "strategies/StrategiesConcretes.hpp"
#include <QCloseEvent>
#include <QDate>
#include <QFrame>
#include <QInputDialog>
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
#include <memory>
#include <vector>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>

// ================================================================
// FONCTIONS UTILITAIRES GLOBALES
// ================================================================
namespace
{
    const QStringList VILLES = {
        "Antananarivo", "Toamasina", "Fianarantsoa", "Mahajanga",
        "Antsirabe", "Toliara", "Antsiranana"
    };

    QStringList villesDuReseau()
    {
        sl::Graphe graphe;
        sl::construireReseauMalgache(graphe);
        QStringList villes;
        for (const std::string& ville : graphe.getVilles())
            villes << QString::fromStdString(ville);
        villes.sort();
        return villes;
    }

    QStringList valeursDeColonne(const QTableWidget* table, int colonne, bool sansDiese = false)
    {
        QStringList liste;
        for (int r = 0; r < table->rowCount(); ++r) {
            const QTableWidgetItem* item = table->item(r, colonne);
            if (!item) continue;
            QString valeur = item->text().trimmed();
            if (sansDiese && valeur.startsWith('#')) valeur.remove(0, 1);
            if (!valeur.isEmpty() && !liste.contains(valeur)) liste << valeur;
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

    QString enMinuscules(const QString& texte) { return texte.trimmed().toLower(); }

    QString groupeParTrois(long long valeur)
    {
        QString chiffres = QString::number(std::llabs(valeur));
        QString resultat;
        int compteur = 0;
        for (int i = chiffres.size() - 1; i >= 0; --i) {
            if (compteur > 0 && compteur % 3 == 0) resultat.prepend(' ');
            resultat.prepend(chiffres[i]);
            ++compteur;
        }
        return valeur < 0 ? "-" + resultat : resultat;
    }
}

// ================================================================
// CONSTRUCTEUR PRINCIPAL
// ================================================================
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      pageTitle_(nullptr), pageSubtitle_(nullptr), pages_(nullptr),
      dashboardButton_(nullptr), commandesButton_(nullptr), colisButton_(nullptr), clientsButton_(nullptr),
      vehiculesButton_(nullptr), chauffeursButton_(nullptr), entrepotsButton_(nullptr), livraisonsButton_(nullptr),
      itinerairesButton_(nullptr), incidentsButton_(nullptr), maintenanceButton_(nullptr), statistiquesButton_(nullptr),
      commandesTable_(nullptr), colisTable_(nullptr), clientsTable_(nullptr), vehiculesTable_(nullptr),
      chauffeursTable_(nullptr), entrepotsTable_(nullptr), livraisonsTable_(nullptr), itinerairesTable_(nullptr),
      incidentsTable_(nullptr), maintenanceTable_(nullptr),
      rechercheCommande_(nullptr), rechercheColis_(nullptr), rechercheClient_(nullptr),
      rechercheVehicule_(nullptr), rechercheChauffeur_(nullptr), rechercheEntrepot_(nullptr),
      rechercheLivraison_(nullptr), rechercheItineraire_(nullptr), rechercheIncident_(nullptr),
      rechercheMaintenance_(nullptr)
{
    setWindowTitle("Smart Logistics");
    resize(1400, 850);
    setStyleSheet(SmartLogisticsStyle::mainStyle());

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    QWidget* sidebar = createSidebar();
    QWidget* content = new QWidget();
    QVBoxLayout* contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(0);
    contentLayout->addWidget(createTopBar());

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

    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(content);

    connect(dashboardButton_, &QPushButton::clicked, this, [this]() { changePage(0); });
    connect(commandesButton_, &QPushButton::clicked, this, [this]() { changePage(1); });
    connect(colisButton_, &QPushButton::clicked, this, [this]() { changePage(2); });
    connect(clientsButton_, &QPushButton::clicked, this, [this]() { changePage(3); });
    connect(vehiculesButton_, &QPushButton::clicked, this, [this]() { changePage(4); });
    connect(chauffeursButton_, &QPushButton::clicked, this, [this]() { changePage(5); });
    connect(entrepotsButton_, &QPushButton::clicked, this, [this]() { changePage(6); });
    connect(livraisonsButton_, &QPushButton::clicked, this, [this]() { changePage(7); });
    connect(itinerairesButton_, &QPushButton::clicked, this, [this]() { changePage(8); });
    connect(incidentsButton_, &QPushButton::clicked, this, [this]() { changePage(9); });
    connect(maintenanceButton_, &QPushButton::clicked, this, [this]() { changePage(10); });
    connect(statistiquesButton_, &QPushButton::clicked, this, [this]() { changePage(11); });

    changePage(0);
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

    QLabel* logo = new QLabel("SMART LOGISTICS"); logo->setObjectName("logo");
    QLabel* subtitle = new QLabel("Fleet Management"); subtitle->setObjectName("sidebarSubtitle");
    layout->addWidget(logo); layout->addWidget(subtitle); layout->addSpacing(25);

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

    layout->addWidget(dashboardButton_); layout->addWidget(commandesButton_);
    layout->addWidget(colisButton_); layout->addWidget(clientsButton_);
    layout->addWidget(vehiculesButton_); layout->addWidget(chauffeursButton_);
    layout->addWidget(entrepotsButton_); layout->addWidget(livraisonsButton_);
    layout->addWidget(itinerairesButton_); layout->addWidget(incidentsButton_);
    layout->addWidget(maintenanceButton_); layout->addWidget(statistiquesButton_);
    layout->addStretch();

    QLabel* systemStatus = new QLabel("● Système opérationnel");
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
    pageTitle_ = new QLabel("Tableau de bord"); pageTitle_->setObjectName("pageTitle");
    pageSubtitle_ = new QLabel("Vue générale de votre activité logistique"); pageSubtitle_->setObjectName("pageSubtitle");
    titleLayout->addWidget(pageTitle_); titleLayout->addWidget(pageSubtitle_);
    layout->addLayout(titleLayout); layout->addStretch();

    etatBaseLabel_ = new QLabel("Base : hors ligne"); etatBaseLabel_->setObjectName("pageSubtitle");
    QPushButton* chargerBase = new QPushButton("Charger depuis la base");
    QPushButton* enregistrerBase = new QPushButton("Enregistrer dans la base");
    connect(chargerBase, &QPushButton::clicked, this, [this]() { chargerDepuisBase(false); });
    connect(enregistrerBase, &QPushButton::clicked, this, [this]() { sauvegarderVersBase(false); });

    layout->addWidget(etatBaseLabel_); layout->addSpacing(12);
    layout->addWidget(chargerBase); layout->addWidget(enregistrerBase); layout->addSpacing(20);

    QLabel* userLabel = new QLabel("Administrateur"); userLabel->setObjectName("userLabel");
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
    scrollArea->setWidgetResizable(true); scrollArea->setFrameShape(QFrame::NoFrame);
    QWidget* content = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(content);
    layout->setContentsMargins(30, 30, 30, 30); layout->setSpacing(25);

    QLabel* sectionTitle = new QLabel("Vue d'ensemble"); sectionTitle->setObjectName("sectionTitle");
    layout->addWidget(sectionTitle);

    QHBoxLayout* statsLayout = new QHBoxLayout(); statsLayout->setSpacing(15);
    auto createCard = [](const QString& title, const QString& value, const QString& description) {
        QFrame* card = new QFrame(); card->setObjectName("statCard");
        QVBoxLayout* cardLayout = new QVBoxLayout(card); cardLayout->setContentsMargins(18, 15, 18, 15);
        QLabel* cardTitle = new QLabel(title); cardTitle->setObjectName("cardTitle");
        QLabel* cardValue = new QLabel(value); cardValue->setObjectName("cardValue");
        QLabel* cardDescription = new QLabel(description); cardDescription->setObjectName("cardDescription");
        cardLayout->addWidget(cardTitle); cardLayout->addWidget(cardValue);
        cardLayout->addWidget(cardDescription); cardLayout->addStretch();
        return card;
    };
    statsLayout->addWidget(createCard("Commandes", "0", "Commandes enregistrées"));
    statsLayout->addWidget(createCard("Livraisons", "0", "Livraisons effectuées"));
    statsLayout->addWidget(createCard("En cours", "0", "Opérations en cours"));
    statsLayout->addWidget(createCard("Incidents", "0", "Incidents à traiter"));
    layout->addLayout(statsLayout);

    QLabel* deliveriesTitle = new QLabel("Livraisons récentes"); deliveriesTitle->setObjectName("sectionTitle");
    layout->addWidget(deliveriesTitle);
    QTableWidget* table = new QTableWidget(); table->setObjectName("dashboardTable");
    table->setColumnCount(5); table->setHorizontalHeaderLabels({"Commande", "Client", "Destination", "Statut", "Date"});
    table->setRowCount(0);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers); table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setAlternatingRowColors(true); table->setMinimumHeight(260);
    layout->addWidget(table);

    QLabel* fleetTitle = new QLabel("État de la flotte"); fleetTitle->setObjectName("sectionTitle");
    layout->addWidget(fleetTitle);
    QHBoxLayout* fleetLayout = new QHBoxLayout(); fleetLayout->setSpacing(15);
    fleetLayout->addWidget(createCard("Véhicules actifs", "0", "En circulation"));
    fleetLayout->addWidget(createCard("En maintenance", "0", "Véhicules immobilisés"));
    fleetLayout->addWidget(createCard("Disponibles", "0", "Prêts à être affectés"));
    layout->addLayout(fleetLayout); layout->addStretch();

    scrollArea->setWidget(content);
    QVBoxLayout* pageLayout = new QVBoxLayout(page); pageLayout->setContentsMargins(0, 0, 0, 0);
    pageLayout->addWidget(scrollArea);
    return page;
}

// ================================================================
// UTILITAIRES CRUD
// ================================================================
static int prochainNumero(QTableWidget* table, const QString& prefixe)
{
    int maximum = 0;
    for (int r = 0; r < table->rowCount(); ++r) {
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
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setAlternatingRowColors(true);
}

// ================================================================
// PAGES (toutes vides au démarrage)
// ================================================================
QWidget* MainWindow::createCommandesPage()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page); layout->setContentsMargins(30, 30, 30, 30); layout->setSpacing(20);
    QLabel* title = new QLabel("Gestion des commandes"); title->setObjectName("sectionTitle"); layout->addWidget(title);
    QHBoxLayout* buttons = new QHBoxLayout(); buttons->addStretch();
    QPushButton* ajouter = new QPushButton("+ Ajouter"); QPushButton* modifier = new QPushButton("Modifier"); QPushButton* supprimer = new QPushButton("Supprimer");
    buttons->addWidget(ajouter); buttons->addWidget(modifier); buttons->addWidget(supprimer); layout->addLayout(buttons);
    QHBoxLayout* search = new QHBoxLayout();
    rechercheCommande_ = new QLineEdit(); rechercheCommande_->setPlaceholderText("Rechercher une commande...");
    QPushButton* rechercher = new QPushButton("Rechercher"); QPushButton* actualiser = new QPushButton("Actualiser");
    search->addWidget(rechercheCommande_); search->addWidget(rechercher); search->addWidget(actualiser); layout->addLayout(search);
    commandesTable_ = new QTableWidget(); commandesTable_->setColumnCount(6);
    
    // [CORRECTION] Affichage du nom du client au lieu du code
    commandesTable_->setHorizontalHeaderLabels({
        "Commande", "Client", "Destination", "Statut", "Date", "Montant"
    });
    
    // [CORRECTION] Données de démo avec des noms
    QStringList data = {
        "#CMD-001|Client A|Antananarivo|Livrée|22/09/2026|125 000 Ar",
        "#CMD-002|Client B|Toamasina|En cours|22/09/2026|280 000 Ar",
        "#CMD-003|Client C|Fianarantsoa|En transit|21/09/2026|175 000 Ar",
        "#CMD-004|Client D|Mahajanga|Livrée|21/09/2026|320 000 Ar",
        "#CMD-005|Client E|Antsirabe|Incident|20/09/2026|150 000 Ar"
    };
    
    commandesTable_->setRowCount(data.size());
    for (int r = 0; r < data.size(); ++r) {
        QStringList values = data[r].split("|");
        for (int c = 0; c < values.size(); ++c) {
            commandesTable_->setItem(r, c, new QTableWidgetItem(values[c]));
        }
    }
    configurerTable(commandesTable_); layout->addWidget(commandesTable_);
    connect(ajouter, &QPushButton::clicked, this, [this]() { ajouterCommande(); });
    connect(modifier, &QPushButton::clicked, this, [this]() { modifierCommande(); });
    connect(supprimer, &QPushButton::clicked, this, [this]() { supprimerCommande(); });
    connect(rechercher, &QPushButton::clicked, this, [this]() { rechercherCommande(); });
    connect(actualiser, &QPushButton::clicked, this, [this]() { actualiserCommandes(); });
    return page;
}

QWidget* MainWindow::createColisPage()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page); layout->setContentsMargins(30, 30, 30, 30); layout->setSpacing(20);
    QLabel* title = new QLabel("Gestion des colis"); title->setObjectName("sectionTitle"); layout->addWidget(title);
    QHBoxLayout* buttons = new QHBoxLayout(); buttons->addStretch();
    QPushButton* ajouter = new QPushButton("+ Ajouter"); QPushButton* modifier = new QPushButton("Modifier"); QPushButton* supprimer = new QPushButton("Supprimer");
    buttons->addWidget(ajouter); buttons->addWidget(modifier); buttons->addWidget(supprimer); layout->addLayout(buttons);
    QHBoxLayout* search = new QHBoxLayout();
    rechercheColis_ = new QLineEdit(); rechercheColis_->setPlaceholderText("Rechercher un colis...");
    QPushButton* rechercher = new QPushButton("Rechercher"); QPushButton* actualiser = new QPushButton("Actualiser");
    search->addWidget(rechercheColis_); search->addWidget(rechercher); search->addWidget(actualiser); layout->addLayout(search);
    colisTable_ = new QTableWidget(); colisTable_->setColumnCount(7);
    colisTable_->setHorizontalHeaderLabels({"ID Colis", "Commande", "Type", "Poids", "Statut", "Destination", "Date"});
    configurerTable(colisTable_); layout->addWidget(colisTable_);
    connect(ajouter, &QPushButton::clicked, this, [this]() { ajouterColis(); });
    connect(modifier, &QPushButton::clicked, this, [this]() { modifierColis(); });
    connect(supprimer, &QPushButton::clicked, this, [this]() { supprimerColis(); });
    connect(rechercher, &QPushButton::clicked, this, [this]() { rechercherColis(); });
    connect(actualiser, &QPushButton::clicked, this, [this]() { actualiserColis(); });
    return page;
}

QWidget* MainWindow::createClientsPage()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page); layout->setContentsMargins(30, 30, 30, 30); layout->setSpacing(20);
    QLabel* title = new QLabel("Gestion des clients"); title->setObjectName("sectionTitle"); layout->addWidget(title);
    QHBoxLayout* buttons = new QHBoxLayout(); buttons->addStretch();
    QPushButton* ajouter = new QPushButton("+ Ajouter"); QPushButton* modifier = new QPushButton("Modifier"); QPushButton* supprimer = new QPushButton("Supprimer");
    buttons->addWidget(ajouter); buttons->addWidget(modifier); buttons->addWidget(supprimer); layout->addLayout(buttons);
    QHBoxLayout* search = new QHBoxLayout();
    rechercheClient_ = new QLineEdit(); rechercheClient_->setPlaceholderText("Rechercher un client...");
    QPushButton* rechercher = new QPushButton("Rechercher"); QPushButton* actualiser = new QPushButton("Actualiser");
    search->addWidget(rechercheClient_); search->addWidget(rechercher); search->addWidget(actualiser); layout->addLayout(search);
    clientsTable_ = new QTableWidget(); clientsTable_->setColumnCount(7);
    clientsTable_->setHorizontalHeaderLabels({"ID", "Prénom", "Nom", "Téléphone", "Email", "Ville", "Commandes"});
    configurerTable(clientsTable_); layout->addWidget(clientsTable_);
    connect(ajouter, &QPushButton::clicked, this, [this]() { ajouterClient(); });
    connect(modifier, &QPushButton::clicked, this, [this]() { modifierClient(); });
    connect(supprimer, &QPushButton::clicked, this, [this]() { supprimerClient(); });
    connect(rechercher, &QPushButton::clicked, this, [this]() { rechercherClient(); });
    connect(actualiser, &QPushButton::clicked, this, [this]() { actualiserClients(); });
    return page;
}

QWidget* MainWindow::createVehiculesPage()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page); layout->setContentsMargins(30, 30, 30, 30); layout->setSpacing(20);
    QLabel* title = new QLabel("Gestion des véhicules"); title->setObjectName("sectionTitle"); layout->addWidget(title);
    QHBoxLayout* buttons = new QHBoxLayout(); buttons->addStretch();
    QPushButton* ajouter = new QPushButton("+ Ajouter"); QPushButton* modifier = new QPushButton("Modifier"); QPushButton* supprimer = new QPushButton("Supprimer");
    buttons->addWidget(ajouter); buttons->addWidget(modifier); buttons->addWidget(supprimer); layout->addLayout(buttons);
    QHBoxLayout* search = new QHBoxLayout();
    rechercheVehicule_ = new QLineEdit(); rechercheVehicule_->setPlaceholderText("Rechercher un véhicule...");
    QPushButton* rechercher = new QPushButton("Rechercher"); QPushButton* actualiser = new QPushButton("Actualiser");
    search->addWidget(rechercheVehicule_); search->addWidget(rechercher); search->addWidget(actualiser); layout->addLayout(search);
    vehiculesTable_ = new QTableWidget(); vehiculesTable_->setColumnCount(9);
    vehiculesTable_->setHorizontalHeaderLabels({"ID", "Immatriculation", "Marque", "Modèle", "Type", "Statut", "Kilométrage", "Capacité", "Carburant"});
    configurerTable(vehiculesTable_); layout->addWidget(vehiculesTable_);
    connect(ajouter, &QPushButton::clicked, this, [this]() { ajouterVehicule(); });
    connect(modifier, &QPushButton::clicked, this, [this]() { modifierVehicule(); });
    connect(supprimer, &QPushButton::clicked, this, [this]() { supprimerVehicule(); });
    connect(rechercher, &QPushButton::clicked, this, [this]() { rechercherVehicule(); });
    connect(actualiser, &QPushButton::clicked, this, [this]() { actualiserVehicules(); });
    return page;
}

QWidget* MainWindow::createChauffeursPage()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page); layout->setContentsMargins(30, 30, 30, 30); layout->setSpacing(20);
    QLabel* title = new QLabel("Gestion des chauffeurs"); title->setObjectName("sectionTitle"); layout->addWidget(title);
    QHBoxLayout* buttons = new QHBoxLayout(); buttons->addStretch();
    QPushButton* ajouter = new QPushButton("+ Ajouter"); QPushButton* modifier = new QPushButton("Modifier"); QPushButton* supprimer = new QPushButton("Supprimer");
    buttons->addWidget(ajouter); buttons->addWidget(modifier); buttons->addWidget(supprimer); layout->addLayout(buttons);
    QHBoxLayout* search = new QHBoxLayout();
    rechercheChauffeur_ = new QLineEdit(); rechercheChauffeur_->setPlaceholderText("Rechercher un chauffeur...");
    QPushButton* rechercher = new QPushButton("Rechercher"); QPushButton* actualiser = new QPushButton("Actualiser");
    search->addWidget(rechercheChauffeur_); search->addWidget(rechercher); search->addWidget(actualiser); layout->addLayout(search);
    chauffeursTable_ = new QTableWidget(); chauffeursTable_->setColumnCount(7);
    chauffeursTable_->setHorizontalHeaderLabels({"ID", "Nom", "Téléphone", "Permis", "Véhicule", "Statut", "Expérience"});
    configurerTable(chauffeursTable_); layout->addWidget(chauffeursTable_);
    connect(ajouter, &QPushButton::clicked, this, [this]() { ajouterChauffeur(); });
    connect(modifier, &QPushButton::clicked, this, [this]() { modifierChauffeur(); });
    connect(supprimer, &QPushButton::clicked, this, [this]() { supprimerChauffeur(); });
    connect(rechercher, &QPushButton::clicked, this, [this]() { rechercherChauffeur(); });
    connect(actualiser, &QPushButton::clicked, this, [this]() { actualiserChauffeurs(); });
    return page;
}

QWidget* MainWindow::createEntrepotsPage()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page); layout->setContentsMargins(30, 30, 30, 30); layout->setSpacing(20);
    QLabel* title = new QLabel("Gestion des entrepôts"); title->setObjectName("sectionTitle"); layout->addWidget(title);
    QHBoxLayout* buttons = new QHBoxLayout(); buttons->addStretch();
    QPushButton* ajouter = new QPushButton("+ Ajouter"); QPushButton* modifier = new QPushButton("Modifier"); QPushButton* supprimer = new QPushButton("Supprimer");
    buttons->addWidget(ajouter); buttons->addWidget(modifier); buttons->addWidget(supprimer); layout->addLayout(buttons);
    QHBoxLayout* search = new QHBoxLayout();
    rechercheEntrepot_ = new QLineEdit(); rechercheEntrepot_->setPlaceholderText("Rechercher un entrepôt...");
    QPushButton* rechercher = new QPushButton("Rechercher"); QPushButton* actualiser = new QPushButton("Actualiser");
    search->addWidget(rechercheEntrepot_); search->addWidget(rechercher); search->addWidget(actualiser); layout->addLayout(search);
    entrepotsTable_ = new QTableWidget(); entrepotsTable_->setColumnCount(6);
    entrepotsTable_->setHorizontalHeaderLabels({"ID", "Nom", "Ville", "Capacité", "Occupation", "Statut"});
    configurerTable(entrepotsTable_); layout->addWidget(entrepotsTable_);
    connect(ajouter, &QPushButton::clicked, this, [this]() { ajouterEntrepot(); });
    connect(modifier, &QPushButton::clicked, this, [this]() { modifierEntrepot(); });
    connect(supprimer, &QPushButton::clicked, this, [this]() { supprimerEntrepot(); });
    connect(rechercher, &QPushButton::clicked, this, [this]() { rechercherEntrepot(); });
    connect(actualiser, &QPushButton::clicked, this, [this]() { actualiserEntrepots(); });
    return page;
}

QWidget* MainWindow::createLivraisonsPage()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page); layout->setContentsMargins(30, 30, 30, 30); layout->setSpacing(20);
    QLabel* title = new QLabel("Gestion des livraisons"); title->setObjectName("sectionTitle"); layout->addWidget(title);
    QHBoxLayout* buttons = new QHBoxLayout(); buttons->addStretch();
    QPushButton* ajouter = new QPushButton("+ Ajouter"); QPushButton* modifier = new QPushButton("Modifier"); QPushButton* supprimer = new QPushButton("Supprimer");
    buttons->addWidget(ajouter); buttons->addWidget(modifier); buttons->addWidget(supprimer); layout->addLayout(buttons);
    QHBoxLayout* search = new QHBoxLayout();
    rechercheLivraison_ = new QLineEdit(); rechercheLivraison_->setPlaceholderText("Rechercher une livraison...");
    QPushButton* rechercher = new QPushButton("Rechercher"); QPushButton* actualiser = new QPushButton("Actualiser");
    search->addWidget(rechercheLivraison_); search->addWidget(rechercher); search->addWidget(actualiser);
    QPushButton* affecter = new QPushButton("Affecter automatiquement");
    connect(affecter, &QPushButton::clicked, this, [this]() { affecterAutomatiquement(); }); search->addWidget(affecter);
    QPushButton* terminer = new QPushButton("Terminer la livraison");
    connect(terminer, &QPushButton::clicked, this, [this]() { terminerLivraison(); }); search->addWidget(terminer);
    layout->addLayout(search);
    livraisonsTable_ = new QTableWidget(); livraisonsTable_->setColumnCount(7);
    livraisonsTable_->setHorizontalHeaderLabels({"ID", "Commande", "Client", "Chauffeur", "Véhicule", "Destination", "Statut"});
    configurerTable(livraisonsTable_); layout->addWidget(livraisonsTable_);
    connect(ajouter, &QPushButton::clicked, this, [this]() { ajouterLivraison(); });
    connect(modifier, &QPushButton::clicked, this, [this]() { modifierLivraison(); });
    connect(supprimer, &QPushButton::clicked, this, [this]() { supprimerLivraison(); });
    connect(rechercher, &QPushButton::clicked, this, [this]() { rechercherLivraison(); });
    connect(actualiser, &QPushButton::clicked, this, [this]() { actualiserLivraisons(); });
    return page;
}

QWidget* MainWindow::createItinerairesPage()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page); layout->setContentsMargins(30, 30, 30, 30); layout->setSpacing(20);
    QLabel* title = new QLabel("Gestion des itinéraires"); title->setObjectName("sectionTitle"); layout->addWidget(title);
    QHBoxLayout* buttons = new QHBoxLayout(); buttons->addStretch();
    QPushButton* ajouter = new QPushButton("+ Ajouter"); QPushButton* modifier = new QPushButton("Modifier"); QPushButton* supprimer = new QPushButton("Supprimer");
    buttons->addWidget(ajouter); buttons->addWidget(modifier); buttons->addWidget(supprimer); layout->addLayout(buttons);
    QHBoxLayout* search = new QHBoxLayout();
    rechercheItineraire_ = new QLineEdit(); rechercheItineraire_->setPlaceholderText("Rechercher un itinéraire...");
    QPushButton* rechercher = new QPushButton("Rechercher"); QPushButton* actualiser = new QPushButton("Actualiser");
    search->addWidget(rechercheItineraire_); search->addWidget(rechercher); search->addWidget(actualiser);
    QPushButton* calculer = new QPushButton("Calculer l'itinéraire");
    connect(calculer, &QPushButton::clicked, this, [this]() { calculerItineraire(); }); search->addWidget(calculer);
    layout->addLayout(search);
    itinerairesTable_ = new QTableWidget(); itinerairesTable_->setColumnCount(7);
    itinerairesTable_->setHorizontalHeaderLabels({"ID", "Départ", "Destination", "Distance", "Durée", "Statut", "Trajet"});
    configurerTable(itinerairesTable_); layout->addWidget(itinerairesTable_);
    connect(ajouter, &QPushButton::clicked, this, [this]() { ajouterItineraire(); });
    connect(modifier, &QPushButton::clicked, this, [this]() { modifierItineraire(); });
    connect(supprimer, &QPushButton::clicked, this, [this]() { supprimerItineraire(); });
    connect(rechercher, &QPushButton::clicked, this, [this]() { rechercherItineraire(); });
    connect(actualiser, &QPushButton::clicked, this, [this]() { actualiserItineraires(); });
    return page;
}

QWidget* MainWindow::createIncidentsPage()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page); layout->setContentsMargins(30, 30, 30, 30); layout->setSpacing(20);
    QLabel* title = new QLabel("Gestion des incidents"); title->setObjectName("sectionTitle"); layout->addWidget(title);
    QHBoxLayout* buttons = new QHBoxLayout(); buttons->addStretch();
    QPushButton* ajouter = new QPushButton("+ Ajouter"); QPushButton* modifier = new QPushButton("Modifier"); QPushButton* supprimer = new QPushButton("Supprimer");
    buttons->addWidget(ajouter); buttons->addWidget(modifier); buttons->addWidget(supprimer); layout->addLayout(buttons);
    QHBoxLayout* search = new QHBoxLayout();
    rechercheIncident_ = new QLineEdit(); rechercheIncident_->setPlaceholderText("Rechercher un incident...");
    QPushButton* rechercher = new QPushButton("Rechercher"); QPushButton* actualiser = new QPushButton("Actualiser");
    search->addWidget(rechercheIncident_); search->addWidget(rechercher); search->addWidget(actualiser); layout->addLayout(search);
    incidentsTable_ = new QTableWidget(); incidentsTable_->setColumnCount(7);
    incidentsTable_->setHorizontalHeaderLabels({"ID", "Date", "Type", "Véhicule", "Chauffeur", "Description", "Statut"});
    configurerTable(incidentsTable_); layout->addWidget(incidentsTable_);
    connect(ajouter, &QPushButton::clicked, this, [this]() { ajouterIncident(); });
    connect(modifier, &QPushButton::clicked, this, [this]() { modifierIncident(); });
    connect(supprimer, &QPushButton::clicked, this, [this]() { supprimerIncident(); });
    connect(rechercher, &QPushButton::clicked, this, [this]() { rechercherIncident(); });
    connect(actualiser, &QPushButton::clicked, this, [this]() { actualiserIncidents(); });
    return page;
}

QWidget* MainWindow::createMaintenancePage()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page); layout->setContentsMargins(30, 30, 30, 30); layout->setSpacing(20);
    QLabel* title = new QLabel("Gestion de la maintenance"); title->setObjectName("sectionTitle"); layout->addWidget(title);
    QHBoxLayout* buttons = new QHBoxLayout(); buttons->addStretch();
    QPushButton* ajouter = new QPushButton("+ Ajouter"); QPushButton* modifier = new QPushButton("Modifier"); QPushButton* supprimer = new QPushButton("Supprimer");
    buttons->addWidget(ajouter); buttons->addWidget(modifier); buttons->addWidget(supprimer); layout->addLayout(buttons);
    QHBoxLayout* search = new QHBoxLayout();
    rechercheMaintenance_ = new QLineEdit(); rechercheMaintenance_->setPlaceholderText("Rechercher une maintenance...");
    QPushButton* rechercher = new QPushButton("Rechercher"); QPushButton* actualiser = new QPushButton("Actualiser");
    search->addWidget(rechercheMaintenance_); search->addWidget(rechercher); search->addWidget(actualiser); layout->addLayout(search);
    maintenanceTable_ = new QTableWidget(); maintenanceTable_->setColumnCount(7);
    maintenanceTable_->setHorizontalHeaderLabels({"ID", "Véhicule", "Type", "Date", "Kilométrage", "Coût", "Statut"});
    configurerTable(maintenanceTable_); layout->addWidget(maintenanceTable_);
    connect(ajouter, &QPushButton::clicked, this, [this]() { ajouterMaintenance(); });
    connect(modifier, &QPushButton::clicked, this, [this]() { modifierMaintenance(); });
    connect(supprimer, &QPushButton::clicked, this, [this]() { supprimerMaintenance(); });
    connect(rechercher, &QPushButton::clicked, this, [this]() { rechercherMaintenance(); });
    connect(actualiser, &QPushButton::clicked, this, [this]() { actualiserMaintenances(); });
    return page;
}

QWidget* MainWindow::createStatistiquesPage()
{
    QWidget* page = new QWidget();
    QScrollArea* scrollArea = new QScrollArea(); scrollArea->setWidgetResizable(true); scrollArea->setFrameShape(QFrame::NoFrame);
    QWidget* content = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(content); layout->setContentsMargins(30, 30, 30, 30); layout->setSpacing(25);
    QLabel* title = new QLabel("Statistiques et performances"); title->setObjectName("sectionTitle"); layout->addWidget(title);
    QHBoxLayout* cards = new QHBoxLayout();
    auto createStatCard = [](const QString& title, const QString& value, const QString& description) {
        QFrame* card = new QFrame(); card->setObjectName("statCard");
        QVBoxLayout* cardLayout = new QVBoxLayout(card);
        cardLayout->addWidget(new QLabel(title)); cardLayout->addWidget(new QLabel(value)); cardLayout->addWidget(new QLabel(description)); cardLayout->addStretch();
        return card;
    };
    cards->addWidget(createStatCard("Taux de livraison", "0 %", "Livraisons réussies"));
    cards->addWidget(createStatCard("Distance parcourue", "0 km", "Ce mois"));
    cards->addWidget(createStatCard("Commandes", "0", "Ce mois"));
    cards->addWidget(createStatCard("Incidents", "0", "Ce mois"));
    layout->addLayout(cards);

    QLabel* performanceTitle = new QLabel("Résumé des performances"); performanceTitle->setObjectName("sectionTitle"); layout->addWidget(performanceTitle);
    QTableWidget* statisticsTable = new QTableWidget(); statisticsTable->setObjectName("dashboardTable");
    statisticsTable->setColumnCount(4); statisticsTable->setHorizontalHeaderLabels({"Indicateur", "Valeur actuelle", "Objectif", "État"});
    statisticsTable->setRowCount(0);
    configurerTable(statisticsTable); statisticsTable->setMinimumHeight(300); layout->addWidget(statisticsTable);
    QLabel* info = new QLabel("Statistiques calculées en temps réel à partir des données de l'application, synchronisées avec PostgreSQL.");
    info->setObjectName("placeholderText"); info->setWordWrap(true); layout->addWidget(info); layout->addStretch();
    scrollArea->setWidget(content);
    QVBoxLayout* pageLayout = new QVBoxLayout(page); pageLayout->setContentsMargins(0, 0, 0, 0); pageLayout->addWidget(scrollArea);
    return page;
}

// ================================================================
// NAVIGATION & RECHERCHE
// ================================================================
QPushButton* MainWindow::createMenuButton(const QString& text)
{
    QPushButton* button = new QPushButton(text);
    button->setObjectName("menuButton"); button->setProperty("selected", false); button->setMinimumHeight(40);
    return button;
}

void MainWindow::changePage(int index)
{
    pages_->setCurrentIndex(index);
    QList<QPushButton*> buttons = {dashboardButton_, commandesButton_, colisButton_, clientsButton_, vehiculesButton_, chauffeursButton_,
                                   entrepotsButton_, livraisonsButton_, itinerairesButton_, incidentsButton_, maintenanceButton_, statistiquesButton_};
    QStringList titles = {"Tableau de bord", "Commandes", "Colis", "Clients", "Véhicules", "Chauffeurs", "Entrepôts", "Livraisons", "Itinéraires", "Incidents", "Maintenance", "Statistiques"};
    QStringList subtitles = {"Vue générale de votre activité logistique", "Gestion et suivi des commandes", "Gestion et suivi des colis", "Gestion des clients",
                             "Gestion de la flotte de véhicules", "Gestion des chauffeurs", "Gestion des entrepôts", "Suivi des livraisons",
                             "Optimisation des itinéraires", "Gestion des incidents", "Suivi de la maintenance", "Analyse des performances"};
    for (int i = 0; i < buttons.size(); ++i) {
        buttons[i]->setProperty("selected", i == index);
        buttons[i]->style()->unpolish(buttons[i]); buttons[i]->style()->polish(buttons[i]);
    }
    if (index >= 0 && index < titles.size()) { pageTitle_->setText(titles[index]); pageSubtitle_->setText(subtitles[index]); }
}

static void rechercherDansTable(QTableWidget* table, const QString& recherche)
{
    for (int row = 0; row < table->rowCount(); ++row) {
        bool found = false;
        for (int column = 0; column < table->columnCount(); ++column) {
            QTableWidgetItem* item = table->item(row, column);
            if (item && item->text().contains(recherche, Qt::CaseInsensitive)) { found = true; break; }
        }
        table->setRowHidden(row, !found && !recherche.isEmpty());
    }
}

// ================================================================
// ACTIONS CRUD
// ================================================================
void MainWindow::ajouterCommande() { ajouterLigne(commandesTable_, "#CMD", "Nouveau : commande"); }
void MainWindow::modifierCommande() { modifierLigne(commandesTable_, "Modifier : commande"); }
void MainWindow::supprimerCommande()
{
    int row = commandesTable_->currentRow();
    if (row < 0) { QMessageBox::information(this, "Suppression", "Sélectionnez d'abord une commande."); return; }
    if (QMessageBox::question(this, "Suppression", "Voulez-vous supprimer cette commande ?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) commandesTable_->removeRow(row);
}
void MainWindow::rechercherCommande() { rechercherDansTable(commandesTable_, rechercheCommande_->text().trimmed()); }
void MainWindow::actualiserCommandes() { rechargerDepuisBase(5); rechercheCommande_->clear(); for (int r = 0; r < commandesTable_->rowCount(); ++r) commandesTable_->setRowHidden(r, false); commandesTable_->clearSelection(); }

void MainWindow::ajouterColis() { ajouterLigne(colisTable_, "COL", "Nouveau : colis"); }
void MainWindow::modifierColis() { modifierLigne(colisTable_, "Modifier : colis"); }
void MainWindow::supprimerColis()
{
    int row = colisTable_->currentRow();
    if (row < 0) { QMessageBox::information(this, "Suppression", "Sélectionnez d'abord un colis."); return; }
    QString destination = texteDe(colisTable_, row, 5);
    if (QMessageBox::question(this, "Suppression", "Voulez-vous supprimer ce colis ?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        colisTable_->removeRow(row);
        mettreAJourStockEntrepot(destination, -1);
    }
}
void MainWindow::rechercherColis() { rechercherDansTable(colisTable_, rechercheColis_->text().trimmed()); }
void MainWindow::actualiserColis() { rechargerDepuisBase(6); rechercheColis_->clear(); for (int r = 0; r < colisTable_->rowCount(); ++r) colisTable_->setRowHidden(r, false); colisTable_->clearSelection(); }

void MainWindow::ajouterClient() { ajouterLigne(clientsTable_, "CLI", "Nouveau : client"); }
void MainWindow::modifierClient() { modifierLigne(clientsTable_, "Modifier : client"); }
void MainWindow::supprimerClient()
{
    int row = clientsTable_->currentRow();
    if (row < 0) { QMessageBox::information(this, "Suppression", "Sélectionnez d'abord un client."); return; }
    if (QMessageBox::question(this, "Suppression", "Voulez-vous supprimer ce client ?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) clientsTable_->removeRow(row);
}
void MainWindow::rechercherClient() { rechercherDansTable(clientsTable_, rechercheClient_->text().trimmed()); }
void MainWindow::actualiserClients() { rechargerDepuisBase(0); rechercheClient_->clear(); for (int r = 0; r < clientsTable_->rowCount(); ++r) clientsTable_->setRowHidden(r, false); clientsTable_->clearSelection(); }

void MainWindow::ajouterVehicule() { ajouterLigne(vehiculesTable_, "VEH", "Nouveau : véhicule"); }
void MainWindow::modifierVehicule() { modifierLigne(vehiculesTable_, "Modifier : véhicule"); }
void MainWindow::supprimerVehicule()
{
    int row = vehiculesTable_->currentRow();
    if (row < 0) { QMessageBox::information(this, "Suppression", "Sélectionnez d'abord un véhicule."); return; }
    if (QMessageBox::question(this, "Suppression", "Voulez-vous supprimer ce véhicule ?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) vehiculesTable_->removeRow(row);
}
void MainWindow::rechercherVehicule() { rechercherDansTable(vehiculesTable_, rechercheVehicule_->text().trimmed()); }
void MainWindow::actualiserVehicules() { rechargerDepuisBase(1); rechercheVehicule_->clear(); for (int r = 0; r < vehiculesTable_->rowCount(); ++r) vehiculesTable_->setRowHidden(r, false); vehiculesTable_->clearSelection(); }

void MainWindow::ajouterChauffeur() { ajouterLigne(chauffeursTable_, "CHA", "Nouveau : chauffeur"); }
void MainWindow::modifierChauffeur() { modifierLigne(chauffeursTable_, "Modifier : chauffeur"); }
void MainWindow::supprimerChauffeur()
{
    int row = chauffeursTable_->currentRow();
    if (row < 0) { QMessageBox::information(this, "Suppression", "Sélectionnez d'abord un chauffeur."); return; }
    if (QMessageBox::question(this, "Suppression", "Voulez-vous supprimer ce chauffeur ?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) chauffeursTable_->removeRow(row);
}
void MainWindow::rechercherChauffeur() { rechercherDansTable(chauffeursTable_, rechercheChauffeur_->text().trimmed()); }
void MainWindow::actualiserChauffeurs() { rechargerDepuisBase(2); rechercheChauffeur_->clear(); for (int r = 0; r < chauffeursTable_->rowCount(); ++r) chauffeursTable_->setRowHidden(r, false); chauffeursTable_->clearSelection(); }

void MainWindow::ajouterEntrepot() { ajouterLigne(entrepotsTable_, "ENT", "Nouveau : entrepôt"); }
void MainWindow::modifierEntrepot() { modifierLigne(entrepotsTable_, "Modifier : entrepôt"); }
void MainWindow::supprimerEntrepot()
{
    int row = entrepotsTable_->currentRow();
    if (row < 0) { QMessageBox::information(this, "Suppression", "Sélectionnez d'abord un entrepôt."); return; }
    if (QMessageBox::question(this, "Suppression", "Voulez-vous supprimer cet entrepôt ?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) entrepotsTable_->removeRow(row);
}
void MainWindow::rechercherEntrepot() { rechercherDansTable(entrepotsTable_, rechercheEntrepot_->text().trimmed()); }
void MainWindow::actualiserEntrepots() { rechargerDepuisBase(3); rechercheEntrepot_->clear(); for (int r = 0; r < entrepotsTable_->rowCount(); ++r) entrepotsTable_->setRowHidden(r, false); entrepotsTable_->clearSelection(); }

void MainWindow::ajouterLivraison() { ajouterLigne(livraisonsTable_, "LIV", "Nouveau : livraison"); }
void MainWindow::modifierLivraison() { modifierLigne(livraisonsTable_, "Modifier : livraison"); }
void MainWindow::supprimerLivraison()
{
    int row = livraisonsTable_->currentRow();
    if (row < 0) { QMessageBox::information(this, "Suppression", "Sélectionnez d'abord une livraison."); return; }
    if (QMessageBox::question(this, "Suppression", "Voulez-vous supprimer cette livraison ?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) livraisonsTable_->removeRow(row);
}
void MainWindow::rechercherLivraison() { rechercherDansTable(livraisonsTable_, rechercheLivraison_->text().trimmed()); }
void MainWindow::actualiserLivraisons() { rechargerDepuisBase(7); rechercheLivraison_->clear(); for (int r = 0; r < livraisonsTable_->rowCount(); ++r) livraisonsTable_->setRowHidden(r, false); livraisonsTable_->clearSelection(); }

void MainWindow::ajouterItineraire() { ajouterLigne(itinerairesTable_, "ITI", "Nouveau : itinéraire"); }
void MainWindow::modifierItineraire() { modifierLigne(itinerairesTable_, "Modifier : itinéraire"); }
void MainWindow::supprimerItineraire()
{
    int row = itinerairesTable_->currentRow();
    if (row < 0) { QMessageBox::information(this, "Suppression", "Sélectionnez d'abord un itinéraire."); return; }
    if (QMessageBox::question(this, "Suppression", "Voulez-vous supprimer cet itinéraire ?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) itinerairesTable_->removeRow(row);
}
void MainWindow::rechercherItineraire() { rechercherDansTable(itinerairesTable_, rechercheItineraire_->text().trimmed()); }
void MainWindow::actualiserItineraires() { rechargerDepuisBase(4); rechercheItineraire_->clear(); for (int r = 0; r < itinerairesTable_->rowCount(); ++r) itinerairesTable_->setRowHidden(r, false); itinerairesTable_->clearSelection(); }

void MainWindow::ajouterIncident() { ajouterLigne(incidentsTable_, "INC", "Nouveau : incident"); }
void MainWindow::modifierIncident() { modifierLigne(incidentsTable_, "Modifier : incident"); }
void MainWindow::supprimerIncident()
{
    int row = incidentsTable_->currentRow();
    if (row < 0) { QMessageBox::information(this, "Suppression", "Sélectionnez d'abord un incident."); return; }
    if (QMessageBox::question(this, "Suppression", "Voulez-vous supprimer cet incident ?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        QStringList ancien; for (int c = 0; c < incidentsTable_->columnCount(); ++c) { const QTableWidgetItem* item = incidentsTable_->item(row, c); ancien << (item ? item->text().trimmed() : QString()); }
        incidentsTable_->removeRow(row); appliquerCoherence(incidentsTable_, -1, ancien, QStringList());
    }
}
void MainWindow::rechercherIncident() { rechercherDansTable(incidentsTable_, rechercheIncident_->text().trimmed()); }
void MainWindow::actualiserIncidents() { rechargerDepuisBase(8); rechercheIncident_->clear(); for (int r = 0; r < incidentsTable_->rowCount(); ++r) incidentsTable_->setRowHidden(r, false); incidentsTable_->clearSelection(); }

void MainWindow::ajouterMaintenance() { ajouterLigne(maintenanceTable_, "MAI", "Nouveau : intervention de maintenance"); }
void MainWindow::modifierMaintenance() { modifierLigne(maintenanceTable_, "Modifier : intervention de maintenance"); }
void MainWindow::supprimerMaintenance()
{
    int row = maintenanceTable_->currentRow();
    if (row < 0) { QMessageBox::information(this, "Suppression", "Sélectionnez d'abord une maintenance."); return; }
    if (QMessageBox::question(this, "Suppression", "Voulez-vous supprimer cette maintenance ?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        QStringList ancien; for (int c = 0; c < maintenanceTable_->columnCount(); ++c) { const QTableWidgetItem* item = maintenanceTable_->item(row, c); ancien << (item ? item->text().trimmed() : QString()); }
        maintenanceTable_->removeRow(row); appliquerCoherence(maintenanceTable_, -1, ancien, QStringList());
    }
}
void MainWindow::rechercherMaintenance() { rechercherDansTable(maintenanceTable_, rechercheMaintenance_->text().trimmed()); }
void MainWindow::actualiserMaintenances() { rechargerDepuisBase(9); rechercheMaintenance_->clear(); for (int r = 0; r < maintenanceTable_->rowCount(); ++r) maintenanceTable_->setRowHidden(r, false); maintenanceTable_->clearSelection(); }

void MainWindow::actualiserStatistiques() {}

// ================================================================
// FORMULAIRES
// ================================================================
QList<ChampFormulaire> MainWindow::champsPour(QTableWidget* table) const
{
    using T = ChampFormulaire::Type;
    auto champ = [](const QString& etiquette, T type = T::Texte, const QStringList& choix = {}, const QString& suffixe = QString(), bool requis = false) {
        ChampFormulaire c; c.etiquette = etiquette; c.type = type; c.choix = choix; c.suffixe = suffixe; c.requis = requis; return c;
    };
    auto avecAucun = [](QStringList liste) { liste.prepend("Aucun"); return liste; };
    auto avecDefaut = [](ChampFormulaire c, const QString& valeur) { c.defaut = valeur; return c; };
    auto avecDecimale = [](ChampFormulaire c) { c.decimal = true; return c; };

    if (table == clientsTable_) return { champ("ID", T::LectureSeule), champ("Prénom", T::Texte, {}, {}, true), champ("Nom", T::Texte, {}, {}, true), champ("Téléphone"), champ("Email"), champ("Ville", T::ChoixEditable, VILLES), champ("Commandes", T::LectureSeule) };
    if (table == vehiculesTable_) return { champ("ID", T::LectureSeule), champ("Immatriculation", T::Texte, {}, {}, true), champ("Marque"), champ("Modèle"), champ("Type", T::ChoixEditable, {"Utilitaire", "Fourgon", "Camion", "Moto", "Frigorifique"}), champ("Statut", T::Choix, {"Disponible", "Actif", "En mission", "Maintenance", "Indisponible"}), champ("Kilométrage", T::Nombre, {}, " km"), avecDefaut(champ("Capacité", T::Nombre, {}, " kg"), "1000 kg"), avecDefaut(champ("Carburant", T::Nombre, {}, " %"), "100 %") };
    if (table == chauffeursTable_) return { champ("ID", T::LectureSeule), champ("Nom", T::Texte, {}, {}, true), champ("Téléphone"), champ("Permis", T::Choix, {"B", "C", "C1", "MOTO"}), champ("Véhicule", T::Choix, avecAucun(valeursDeColonne(vehiculesTable_, 0))), champ("Statut", T::Choix, {"Disponible", "En livraison", "En congé", "Indisponible"}), avecDefaut(champ("Expérience", T::Nombre, {}, " ans"), "3 ans") };
    if (table == entrepotsTable_) return { champ("ID", T::LectureSeule), champ("Nom", T::Texte, {}, {}, true), champ("Ville", T::ChoixEditable, VILLES), champ("Capacité", T::Nombre, {}, " colis"), champ("Occupation", T::Nombre, {}, " colis"), champ("Statut", T::Choix, {"Actif", "Disponible", "Presque plein", "Plein", "Inactif"}) };
    if (table == itinerairesTable_) return { champ("ID", T::LectureSeule), champ("Départ", T::Choix, villesDuReseau(), {}, true), champ("Destination", T::Choix, villesDuReseau(), {}, true), champ("Distance", T::Nombre, {}, " km"), champ("Durée (ex : 8h 30)"), champ("Statut", T::Choix, {"Planifié", "Actif", "Optimisé", "Terminé"}), champ("Trajet (calculé)", T::LectureSeule) };
    
    // [CORRECTION] Utilisation de la colonne 2 (Nom) pour le client
    if (table == commandesTable_) return { champ("Commande", T::LectureSeule), champ("Client", T::Choix, valeursDeColonne(clientsTable_, 2), {}, true), champ("Destination", T::ChoixEditable, VILLES), champ("Statut", T::Choix, {"En attente", "En cours", "En transit", "Livrée", "Incident", "Annulée"}), champ("Date", T::Date), champ("Montant", T::Nombre, {}, " Ar") };
    
    if (table == colisTable_) return { champ("ID Colis", T::LectureSeule), champ("Commande", T::Choix, valeursDeColonne(commandesTable_, 0, true), {}, true), champ("Type", T::ChoixEditable, {"Standard", "Fragile", "Volumineux"}), avecDecimale(champ("Poids", T::Nombre, {}, " kg")), champ("Statut", T::Choix, {"En attente", "En cours", "En transit", "Livré", "Incident"}), champ("Destination", T::ChoixEditable, VILLES), champ("Date", T::Date) };
    
    // [CORRECTION] Utilisation de la colonne 1 (Nom/Immatriculation) pour chauffeur et véhicule
    if (table == livraisonsTable_) return { champ("ID", T::LectureSeule), champ("Commande", T::Choix, valeursDeColonne(commandesTable_, 0, true), {}, true), champ("Client (selon la commande)", T::LectureSeule), champ("Chauffeur", T::Choix, avecAucun(valeursDeColonne(chauffeursTable_, 1))), champ("Véhicule", T::Choix, avecAucun(valeursDeColonne(vehiculesTable_, 1))), champ("Destination", T::ChoixEditable, VILLES), champ("Statut", T::Choix, {"Planifiée", "En cours", "En transit", "Livrée", "Incident", "Annulée"}) };
    
    // [CORRECTION] Utilisation de la colonne 1 (Immatriculation/Nom) pour véhicule et chauffeur
    if (table == incidentsTable_) return { champ("ID", T::LectureSeule), champ("Date", T::Date), champ("Type", T::ChoixEditable, {"Panne", "Accident", "Retard", "Autre"}, {}, true), champ("Véhicule", T::Choix, avecAucun(valeursDeColonne(vehiculesTable_, 1))), champ("Chauffeur", T::Choix, avecAucun(valeursDeColonne(chauffeursTable_, 1))), champ("Description", T::Texte, {}, {}, true), champ("Statut", T::Choix, {"Ouvert", "Traité", "Clos"}) };
    
    // [CORRECTION] Utilisation de la colonne 1 (Immatriculation) pour véhicule
    if (table == maintenanceTable_) return { champ("ID", T::LectureSeule), champ("Véhicule", T::Choix, valeursDeColonne(vehiculesTable_, 1), {}, true), champ("Type", T::ChoixEditable, {"Révision moteur", "Vidange", "Freins", "Pneus", "Révision générale"}, {}, true), champ("Date", T::Date), champ("Kilométrage", T::Nombre, {}, " km"), champ("Coût", T::Nombre, {}, " Ar"), champ("Statut", T::Choix, {"Planifiée", "En cours", "Terminée"}) };
    
    return {};
}

void MainWindow::completerValeursDerivees(QTableWidget* table, QStringList& valeurs) const
{
    if (table == livraisonsTable_ && valeurs.size() > 2) {
        QString client;
        for (int r = 0; r < commandesTable_->rowCount(); ++r) {
            QString code = texteDe(commandesTable_, r, 0); if (code.startsWith('#')) code.remove(0, 1);
            if (code == valeurs[1]) { client = texteDe(commandesTable_, r, 1); break; }
        }
        valeurs[2] = client;
    }
    if (table == clientsTable_ && valeurs.size() > 6) {
        int nombre = 0; const QString nomClient = valeurs[2];
        for (int r = 0; r < commandesTable_->rowCount(); ++r) { if (texteDe(commandesTable_, r, 1) == nomClient) ++nombre; }
        valeurs[6] = QString::number(nombre);
    }
}

void MainWindow::ajouterLigne(QTableWidget* table, const QString& prefixe, const QString& titre)
{
    using T = ChampFormulaire::Type;
    const QList<ChampFormulaire> champs = champsPour(table); if (champs.isEmpty()) return;
    QStringList initiales;
    for (const ChampFormulaire& champ : champs) {
        switch (champ.type) {
            case T::Choix: case T::ChoixEditable: initiales << (champ.choix.isEmpty() ? QString() : champ.choix.first()); break;
            case T::Date: initiales << QDate::currentDate().toString("dd/MM/yyyy"); break;
            case T::Nombre: initiales << ("0" + champ.suffixe); break;
            default: initiales << QString(); break;
        }
    }
    for (int c = 0; c < champs.size(); ++c) { if (!champs[c].defaut.isEmpty()) initiales[c] = champs[c].defaut; }
    initiales[0] = prochainCode(table, prefixe); if (table == clientsTable_) initiales[6] = "0";
    FormulaireDialog dialogue(titre, champs, initiales, this);
    if (dialogue.exec() != QDialog::Accepted) return;
    QStringList valeurs = dialogue.valeurs(); valeurs[0] = initiales[0]; completerValeursDerivees(table, valeurs);
    const int ligne = table->rowCount(); table->insertRow(ligne);
    for (int c = 0; c < valeurs.size() && c < table->columnCount(); ++c) table->setItem(ligne, c, new QTableWidgetItem(valeurs[c]));
    table->selectRow(ligne); table->scrollToItem(table->item(ligne, 0));
    appliquerCoherence(table, ligne, QStringList(), valeurs);

    if (table == colisTable_ && valeurs.size() > 5) {
        QString destination = valeurs[5];
        mettreAJourStockEntrepot(destination, 1);
    }
}

void MainWindow::modifierLigne(QTableWidget* table, const QString& titre)
{
    const int ligne = table->currentRow();
    if (ligne < 0) { QMessageBox::information(this, "Modification", "Sélectionnez d'abord une ligne dans le tableau."); return; }
    const QList<ChampFormulaire> champs = champsPour(table); if (champs.isEmpty()) return;
    QStringList initiales; for (int c = 0; c < champs.size(); ++c) initiales << texteDe(table, ligne, c);
    FormulaireDialog dialogue(titre, champs, initiales, this);
    if (dialogue.exec() != QDialog::Accepted) return;
    QStringList valeurs = dialogue.valeurs(); valeurs[0] = initiales[0]; completerValeursDerivees(table, valeurs);

    if (table == commandesTable_) {
        const QString ancienStatut = initiales[3];
        const QString nouveauStatut = valeurs[3];
        if (ancienStatut != nouveauStatut) {
            if (nouveauStatut == "Annulée" && (ancienStatut == "En cours" || ancienStatut == "En transit" || ancienStatut == "Livrée")) {
                QMessageBox::warning(this, "Transition interdite (State Pattern)", "Une commande ne peut pas être annulée une fois qu'elle est en cours, en transit ou livrée."); return;
            }
            if (nouveauStatut == "En attente" && (ancienStatut == "En transit" || ancienStatut == "Livrée")) {
                QMessageBox::warning(this, "Transition interdite (State Pattern)", "Impossible de rétrograder une commande déjà en transit ou livrée vers « En attente »."); return;
            }
            if (nouveauStatut == "Livrée" && ancienStatut == "Annulée") {
                QMessageBox::warning(this, "Transition interdite (State Pattern)", "Une commande annulée ne peut pas être marquée comme livrée."); return;
            }
        }
    }

    if (table == itinerairesTable_ && valeurs.size() > 6 && initiales.size() > 2 && (valeurs[1] != initiales[1] || valeurs[2] != initiales[2])) {
        valeurs[6].clear();
    }

    if (table == clientsTable_ && initiales.size() > 2 && valeurs.size() > 2 && valeurs[2] != initiales[2]) {
        for (int r = 0; r < commandesTable_->rowCount(); ++r) {
            if (texteDe(commandesTable_, r, 1) == initiales[2]) {
                commandesTable_->item(r, 1)->setText(valeurs[2]);
            }
        }
    }

    if (table == colisTable_ && valeurs.size() > 5 && initiales.size() > 5) {
        QString ancienneDestination = initiales[5];
        QString nouvelleDestination = valeurs[5];
        if (ancienneDestination != nouvelleDestination) {
            mettreAJourStockEntrepot(ancienneDestination, -1);
            mettreAJourStockEntrepot(nouvelleDestination, 1);
        }
    }

    for (int c = 0; c < valeurs.size() && c < table->columnCount(); ++c) {
        QTableWidgetItem* item = table->item(ligne, c);
        if (item) item->setText(valeurs[c]); else table->setItem(ligne, c, new QTableWidgetItem(valeurs[c]));
    }
    appliquerCoherence(table, ligne, initiales, valeurs);
}

void MainWindow::mettreAJourStockEntrepot(const QString& ville, int delta)
{
    if (!entrepotsTable_ || ville.isEmpty()) return;
    for (int r = 0; r < entrepotsTable_->rowCount(); ++r) {
        QString villeEntrepot = texteDe(entrepotsTable_, r, 2);
        if (villeEntrepot.compare(ville, Qt::CaseInsensitive) == 0) {
            QString stockActuel = texteDe(entrepotsTable_, r, 4);
            int stock = static_cast<int>(nombreDe(stockActuel));
            stock += delta;
            if (stock < 0) stock = 0;
            QTableWidgetItem* item = entrepotsTable_->item(r, 4);
            if (item) item->setText(QString("%1 colis").arg(stock));
            break;
        }
    }
}

// ================================================================
// BASE DE DONNÉES POSTGRESQL
// ================================================================
namespace {
    const char* libelleTable(int identifiant) {
        static const char* const libelles[NOMBRE_TABLES] = {"clients", "véhicules", "chauffeurs", "entrepôts", "itinéraires", "commandes", "colis", "livraisons", "incidents", "maintenance"};
        return libelles[identifiant];
    }
}

QTableWidget* MainWindow::tablePour(int identifiant) const
{
    switch (static_cast<TableId>(identifiant)) {
        case TableId::Clients: return clientsTable_; case TableId::Vehicules: return vehiculesTable_; case TableId::Chauffeurs: return chauffeursTable_;
        case TableId::Entrepots: return entrepotsTable_; case TableId::Itineraires: return itinerairesTable_; case TableId::Commandes: return commandesTable_;
        case TableId::Colis: return colisTable_; case TableId::Livraisons: return livraisonsTable_; case TableId::Incidents: return incidentsTable_;
        case TableId::Maintenance: return maintenanceTable_;
    }
    return nullptr;
}

void MainWindow::initialiserBaseDeDonnees()
{
    baseDeDonnees_ = std::make_unique<GuiDatabase>();
    sauvegardeTimer_ = new QTimer(this); sauvegardeTimer_->setSingleShot(true); sauvegardeTimer_->setInterval(700);
    connect(sauvegardeTimer_, &QTimer::timeout, this, [this]() { sauvegarderVersBase(true); });
    statistiquesTimer_ = new QTimer(this); statistiquesTimer_->setSingleShot(true); statistiquesTimer_->setInterval(150);
    connect(statistiquesTimer_, &QTimer::timeout, this, [this]() { actualiserTableauDeBord(); });
    for (int i = 0; i < NOMBRE_TABLES; ++i) surveillerTable(tablePour(i), i);
    chargerDepuisBase(true); actualiserTableauDeBord();
}

void MainWindow::surveillerTable(QTableWidget* table, int identifiant)
{
    auto declencheur = [this, identifiant]() { planifierSauvegarde(identifiant); statistiquesTimer_->start(); };
    connect(table->model(), &QAbstractItemModel::rowsInserted, this, declencheur);
    connect(table->model(), &QAbstractItemModel::rowsRemoved,  this, declencheur);
    connect(table->model(), &QAbstractItemModel::dataChanged,  this, declencheur);
}

void MainWindow::planifierSauvegarde(int identifiant)
{
    if (synchronisationEnCours_ || !autoSauvegarde_) return;
    tablesModifiees_ |= (1u << identifiant); sauvegardeTimer_->start();
}

void MainWindow::afficherEtatBase(const QString& texte, bool erreur)
{
    etatBaseLabel_->setText(texte);
    etatBaseLabel_->setStyleSheet(erreur ? "color: #dc2626; font-weight: 600;" : "color: #16a34a; font-weight: 600;");
}

void MainWindow::chargerDepuisBase(bool silencieux)
{
    QStringList erreurs; QStringList details; bool vide[NOMBRE_TABLES] = {}; int totalLignes = 0; synchronisationEnCours_ = true;
    for (int i = 0; i < NOMBRE_TABLES; ++i) {
        const ResultatSync resultat = baseDeDonnees_->charger(static_cast<TableId>(i), tablePour(i), false);
        if (!resultat.ok) { erreurs << resultat.erreurs; break; }
        vide[i] = (resultat.lignes == 0); totalLignes += resultat.lignes; details << QString("%1 : %2 ligne(s)").arg(libelleTable(i)).arg(resultat.lignes);
    }
    const bool baseVide = erreurs.isEmpty() && totalLignes == 0;
    if (erreurs.isEmpty() && !baseVide) { for (int i = 0; i < NOMBRE_TABLES; ++i) { if (vide[i]) tablePour(i)->setRowCount(0); } }
    synchronisationEnCours_ = false;
    if (!erreurs.isEmpty()) {
        autoSauvegarde_ = false; afficherEtatBase("Base : hors ligne", true); etatBaseLabel_->setToolTip(erreurs.join("\n"));
        if (!silencieux) QMessageBox::warning(this, "PostgreSQL", "Connexion impossible :\n" + erreurs.join("\n"));
        actualiserTableauDeBord(); return;
    }
    autoSauvegarde_ = true; afficherEtatBase("Base de donnees: connecté", false); etatBaseLabel_->setToolTip(details.join("\n"));
    if (baseVide) { details << "Base vide : prête à recevoir vos données."; }
    actualiserTableauDeBord(); if (!silencieux) QMessageBox::information(this, "PostgreSQL", details.join("\n"));
}

void MainWindow::rechargerDepuisBase(int identifiant)
{
    if (!baseDeDonnees_) return;
    if (sauvegardeTimer_ && sauvegardeTimer_->isActive()) { sauvegardeTimer_->stop(); sauvegarderVersBase(true); }
    synchronisationEnCours_ = true;
    const ResultatSync resultat = baseDeDonnees_->charger(static_cast<TableId>(identifiant), tablePour(identifiant), true);
    synchronisationEnCours_ = false;
    if (resultat.ok) { autoSauvegarde_ = true; afficherEtatBase("Base : PostgreSQL connecté", false); etatBaseLabel_->setToolTip(QString()); }
    else { afficherEtatBase("Base : hors ligne", true); etatBaseLabel_->setToolTip(resultat.erreurs.join("\n")); }
    actualiserTableauDeBord();
}

void MainWindow::sauvegarderVersBase(bool silencieux)
{
    const unsigned masque = silencieux ? tablesModifiees_ : ~0u; tablesModifiees_ = 0;
    QStringList erreurs; bool connexionPerdue = false; unsigned aRecharger = 0; int total = 0;
    for (int i = 0; i < NOMBRE_TABLES; ++i) {
        if ((masque & (1u << i)) == 0) continue;
        const ResultatSync resultat = baseDeDonnees_->sauvegarder(static_cast<TableId>(i), tablePour(i)); total += resultat.lignes;
        for (const QString& erreur : resultat.erreurs) erreurs << QString("%1 — %2").arg(libelleTable(i), erreur);
        if (resultat.suppressionRefusee) aRecharger |= (1u << i);
        if (!resultat.ok) { connexionPerdue = true; break; }
    }
    if (aRecharger != 0) {
        synchronisationEnCours_ = true;
        for (int i = 0; i < NOMBRE_TABLES; ++i) { if (aRecharger & (1u << i)) baseDeDonnees_->charger(static_cast<TableId>(i), tablePour(i), true); }
        synchronisationEnCours_ = false;
    }
    if (connexionPerdue) { autoSauvegarde_ = false; afficherEtatBase("Base : hors ligne", true); etatBaseLabel_->setToolTip(erreurs.join("\n")); }
    else { autoSauvegarde_ = true; afficherEtatBase(erreurs.isEmpty() ? "Base : enregistré" : "Base : enregistré avec erreurs", !erreurs.isEmpty()); etatBaseLabel_->setToolTip(erreurs.join("\n")); }
    if (!erreurs.isEmpty()) QMessageBox::warning(this, "PostgreSQL", erreurs.join("\n"));
    else if (!silencieux) QMessageBox::information(this, "PostgreSQL", QString("%1 ligne(s) enregistrée(s) dans la base.").arg(total));
    actualiserTableauDeBord();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (sauvegardeTimer_ && sauvegardeTimer_->isActive() && autoSauvegarde_) { sauvegardeTimer_->stop(); sauvegarderVersBase(true); }
    QMainWindow::closeEvent(event);
}

// ================================================================
// TABLEAU DE BORD ET STATISTIQUES
// ================================================================
void MainWindow::actualiserTableauDeBord()
{
    if (!pages_ || !commandesTable_ || !livraisonsTable_ || !incidentsTable_ || !vehiculesTable_ || !entrepotsTable_) return;
    auto compter = [](const QTableWidget* table, int colonne, const QStringList& statuts) {
        int nombre = 0; for (int r = 0; r < table->rowCount(); ++r) { if (statuts.contains(enMinuscules(texteDe(table, r, colonne)))) ++nombre; } return nombre;
    };
    auto pourcentage = [](double partie, double total) { return total > 0 ? partie * 100.0 / total : 0.0; };
    auto etat = [](double valeur, double objectif) { if (valeur >= objectif) return QString("Objectif atteint"); if (valeur >= objectif * 0.9) return QString("Presque atteint"); return QString("En progression"); };
    auto groupe = [](long long valeur) {
        QString chiffres = QString::number(valeur); QString resultat; int n = 0;
        for (int i = chiffres.size() - 1; i >= 0; --i) { if (n > 0 && n % 3 == 0) resultat.prepend(' '); resultat.prepend(chiffres[i]); ++n; } return resultat;
    };
    const int nbCommandes = commandesTable_->rowCount(), nbLivraisons = livraisonsTable_->rowCount(), nbIncidents = incidentsTable_->rowCount(), nbVehicules = vehiculesTable_->rowCount();
    const int livrees = compter(livraisonsTable_, 6, {"livrée", "livree", "livré"}), enCours = compter(livraisonsTable_, 6, {"en cours", "en transit"});
    const int ouverts = compter(incidentsTable_, 6, {"ouvert"}), resolus = compter(incidentsTable_, 6, {"traité", "traite", "clos"});
    const int horsService = compter(vehiculesTable_, 5, {"maintenance", "indisponible", "en maintenance"}), enActivite = compter(vehiculesTable_, 5, {"actif", "en mission", "en service"});
    double kmTotal = 0; for (int r = 0; r < vehiculesTable_->rowCount(); ++r) kmTotal += nombreDe(texteDe(vehiculesTable_, r, 6));
    double stock = 0, capacite = 0; for (int r = 0; r < entrepotsTable_->rowCount(); ++r) { capacite += nombreDe(texteDe(entrepotsTable_, r, 3)); stock += nombreDe(texteDe(entrepotsTable_, r, 4)); }
    const double tauxLivraison = pourcentage(livrees, nbLivraisons), tauxIncidents = pourcentage(resolus, nbIncidents), tauxFlotte = pourcentage(enActivite, nbVehicules), tauxEntrepots = pourcentage(stock, capacite);

    QWidget* tableauDeBord = pages_->widget(0);
    const QList<QLabel*> valeurs = tableauDeBord->findChildren<QLabel*>("cardValue");
    if (valeurs.size() >= 4) { valeurs[0]->setText(QString::number(nbCommandes)); valeurs[1]->setText(QString::number(livrees)); valeurs[2]->setText(QString::number(enCours)); valeurs[3]->setText(QString::number(ouverts)); }
    if (valeurs.size() >= 7) { valeurs[4]->setText(QString::number(enActivite)); valeurs[5]->setText(QString::number(compter(vehiculesTable_, 5, {"maintenance", "en maintenance"}))); valeurs[6]->setText(QString::number(compter(vehiculesTable_, 5, {"disponible"}))); }
    if (QTableWidget* recentes = tableauDeBord->findChild<QTableWidget*>("dashboardTable")) {
        QList<int> lignes; for (int r = 0; r < commandesTable_->rowCount(); ++r) lignes << r;
        std::stable_sort(lignes.begin(), lignes.end(), [this](int a, int b) { return QDate::fromString(texteDe(commandesTable_, a, 4), "dd/MM/yyyy") > QDate::fromString(texteDe(commandesTable_, b, 4), "dd/MM/yyyy"); });
        const int affichees = qMin(5, lignes.size()); recentes->setRowCount(affichees);
        for (int i = 0; i < affichees; ++i) for (int c = 0; c < 5; ++c) recentes->setItem(i, c, new QTableWidgetItem(texteDe(commandesTable_, lignes[i], c)));
    }

    QWidget* statistiques = pages_->widget(11);
    const QList<QLabel*> valeursStat = statistiques->findChildren<QLabel*>("cardValue");
    const QList<QLabel*> descriptionsStat = statistiques->findChildren<QLabel*>("cardDescription");
    if (valeursStat.size() >= 4 && descriptionsStat.size() >= 4) {
        valeursStat[0]->setText(QString("%1 %").arg(qRound(tauxLivraison))); descriptionsStat[0]->setText("Livraisons réussies");
        valeursStat[1]->setText(groupe(qRound64(kmTotal)) + " km"); descriptionsStat[1]->setText("Kilométrage total de la flotte");
        valeursStat[2]->setText(QString::number(nbCommandes)); descriptionsStat[2]->setText("Commandes enregistrées");
        valeursStat[3]->setText(QString::number(nbIncidents)); descriptionsStat[3]->setText("Incidents enregistrés");
    }
    if (QTableWidget* tableStat = statistiques->findChild<QTableWidget*>("dashboardTable")) {
        struct Ligne { QString indicateur, valeur, objectif, etat; };
        const QList<Ligne> lignes = {{"Livraisons réussies", QString("%1 %").arg(qRound(tauxLivraison)), "95 %", etat(tauxLivraison, 95)},
                                     {"Véhicules disponibles", QString::number(nbVehicules - horsService), QString::number(nbVehicules), etat(nbVehicules - horsService, nbVehicules)},
                                     {"Incidents résolus", QString("%1 %").arg(qRound(tauxIncidents)), "90 %", etat(tauxIncidents, 90)},
                                     {"Utilisation flotte", QString("%1 %").arg(qRound(tauxFlotte)), "80 %", etat(tauxFlotte, 80)},
                                     {"Occupation entrepôts", QString("%1 %").arg(qRound(tauxEntrepots)), "80 %", etat(tauxEntrepots, 80)}};
        tableStat->setRowCount(lignes.size());
        for (int r = 0; r < lignes.size(); ++r) { tableStat->setItem(r, 0, new QTableWidgetItem(lignes[r].indicateur)); tableStat->setItem(r, 1, new QTableWidgetItem(lignes[r].valeur)); tableStat->setItem(r, 2, new QTableWidgetItem(lignes[r].objectif)); tableStat->setItem(r, 3, new QTableWidgetItem(lignes[r].etat)); }
    }
    if (QLabel* info = statistiques->findChild<QLabel*>("placeholderText")) {
        info->setText(autoSauvegarde_ ? "Statistiques calculées à partir des données de l'application, synchronisées avec PostgreSQL." : "Mode hors ligne : statistiques calculées à partir des données de démonstration affichées.");
    }

    verifierAlertesMaintenance();
}

// ================================================================
// AFFECTATION AUTOMATIQUE & FIN DE LIVRAISON
// ================================================================
void MainWindow::affecterAutomatiquement()
{
    const int ligne = livraisonsTable_->currentRow();
    if (ligne < 0) { QMessageBox::information(this, "Affectation", "Sélectionnez d'abord une livraison « Planifiée » dans le tableau."); return; }
    const QString statut = enMinuscules(texteDe(livraisonsTable_, ligne, 6));
    if (statut != "planifiée" && statut != "planifiee") { QMessageBox::information(this, "Affectation", "Seule une livraison « Planifiée » peut être affectée automatiquement.\nStatut actuel : " + texteDe(livraisonsTable_, ligne, 6)); return; }
    auto sansCette = [](QString texte) { texte = texte.trimmed(); if (texte.startsWith('#')) texte.remove(0, 1); return texte; };
    const QString codeCommande = sansCette(texteDe(livraisonsTable_, ligne, 1));
    const QString destination  = texteDe(livraisonsTable_, ligne, 5);
    double poids = 0.0; int nombreColis = 0; bool frigorifique = false;
    for (int r = 0; r < colisTable_->rowCount(); ++r) {
        if (sansCette(texteDe(colisTable_, r, 1)) != codeCommande) continue;
        ++nombreColis; poids += nombreDe(texteDe(colisTable_, r, 3));
        const QString type = enMinuscules(texteDe(colisTable_, r, 2));
        if (type.contains("frigo") || type.contains("réfrig") || type.contains("refrig")) frigorifique = true;
    }
    if (nombreColis == 0) { QMessageBox::information(this, "Affectation", QString("Aucun colis n'est rattaché à la commande %1.\nAjoutez d'abord ses colis dans la page Colis.").arg(codeCommande)); return; }
    try {
        sl::Commande commande(codeCommande.toStdString(), nullptr, destination.toStdString());
        sl::ParametresColis parametresColis; parametresColis.id = codeCommande.toStdString() + "-LOT"; parametresColis.description = "Colis de la commande";
        parametresColis.poidsKg = poids; parametresColis.volumeM3 = 0.1; parametresColis.origine = "Entrepôt"; parametresColis.destination = destination.toStdString();
        if (frigorifique) { parametresColis.temperatureMinC = 2.0; parametresColis.temperatureMaxC = 8.0; }
        commande.ajouterColis(sl::ColisFactory::creer(frigorifique ? "REFRIGERE" : "STANDARD", parametresColis));
        std::vector<std::unique_ptr<sl::Vehicule>> vehicules; std::vector<sl::Vehicule*> poolVehicules; std::vector<int> ligneDuVehicule;
        for (int r = 0; r < vehiculesTable_->rowCount(); ++r) {
            if (enMinuscules(texteDe(vehiculesTable_, r, 5)) != "disponible") continue;
            std::string type = texteDe(vehiculesTable_, r, 4).toUpper().replace(' ', '_').toStdString(); if (type == "UTILITAIRE") type = "FOURGON";
            sl::ParametresVehicule p; p.immatriculation = texteDe(vehiculesTable_, r, 1).toStdString(); p.marque = texteDe(vehiculesTable_, r, 2).toStdString();
            p.modele = texteDe(vehiculesTable_, r, 3).toStdString(); p.kilometrage = nombreDe(texteDe(vehiculesTable_, r, 6)); p.capaciteKg = nombreDe(texteDe(vehiculesTable_, r, 7)); p.niveauCarburantPct = nombreDe(texteDe(vehiculesTable_, r, 8));
            try { vehicules.push_back(sl::VehiculeFactory::creer(type, p)); poolVehicules.push_back(vehicules.back().get()); ligneDuVehicule.push_back(r); } catch (const std::exception&) {}
        }
        std::vector<std::unique_ptr<sl::Chauffeur>> chauffeurs; std::vector<sl::Chauffeur*> poolChauffeurs; std::vector<int> ligneDuChauffeur;
        for (int r = 0; r < chauffeursTable_->rowCount(); ++r) {
            if (enMinuscules(texteDe(chauffeursTable_, r, 5)) != "disponible") continue;
            const QString permis = texteDe(chauffeursTable_, r, 3).toUpper(); sl::TypePermis typePermis = sl::TypePermis::B;
            if (permis == "C") typePermis = sl::TypePermis::C; if (permis == "C1") typePermis = sl::TypePermis::C1; if (permis == "MOTO") typePermis = sl::TypePermis::MOTO;
            chauffeurs.push_back(std::make_unique<sl::Chauffeur>(texteDe(chauffeursTable_, r, 0).toStdString(), texteDe(chauffeursTable_, r, 1).toStdString(), typePermis, static_cast<int>(nombreDe(texteDe(chauffeursTable_, r, 6)))));
            poolChauffeurs.push_back(chauffeurs.back().get()); ligneDuChauffeur.push_back(r);
        }
        if (poolVehicules.empty()) { QMessageBox::warning(this, "Affectation impossible", "Aucun véhicule n'est disponible."); return; }
        if (poolChauffeurs.empty()) { QMessageBox::warning(this, "Affectation impossible", "Aucun chauffeur n'est disponible."); return; }
        const sl::ResultatAffectation resultat = sl::AllocateurRessources::affecter(commande, poolVehicules, poolChauffeurs);
        if (!resultat.reussie) { QMessageBox::warning(this, "Affectation impossible", QString::fromStdString(resultat.raisonEchec)); return; }
        int ligneVehicule = -1; for (std::size_t i = 0; i < poolVehicules.size(); ++i) { if (poolVehicules[i] == resultat.vehicule) ligneVehicule = ligneDuVehicule[i]; }
        int ligneChauffeur = -1; for (std::size_t i = 0; i < poolChauffeurs.size(); ++i) { if (poolChauffeurs[i] == resultat.chauffeur) ligneChauffeur = ligneDuChauffeur[i]; }
        const QString codeVehicule  = texteDe(vehiculesTable_, ligneVehicule, 0);
        const QString codeChauffeur = texteDe(chauffeursTable_, ligneChauffeur, 0);
        auto definir = [](QTableWidget* table, int l, int c, const QString& valeur) { if (QTableWidgetItem* item = table->item(l, c)) item->setText(valeur); else table->setItem(l, c, new QTableWidgetItem(valeur)); };
        definir(livraisonsTable_, ligne, 3, codeChauffeur); definir(livraisonsTable_, ligne, 4, codeVehicule); definir(livraisonsTable_, ligne, 6, "En cours");
        definir(vehiculesTable_,  ligneVehicule,  5, "En mission"); definir(chauffeursTable_, ligneChauffeur, 4, codeVehicule); definir(chauffeursTable_, ligneChauffeur, 5, "En livraison");
        for (int r = 0; r < commandesTable_->rowCount(); ++r) { if (sansCette(texteDe(commandesTable_, r, 0)) == codeCommande) definir(commandesTable_, r, 3, "En cours"); }
        QMessageBox::information(this, "Affectation réussie", QString("Livraison %1 (commande %2 : %3 kg%4)\n\nVéhicule : %5\nChauffeur : %6\nScore : %7")
            .arg(texteDe(livraisonsTable_, ligne, 0), codeCommande).arg(poids, 0, 'f', 1).arg(frigorifique ? ", froid" : "").arg(codeVehicule, codeChauffeur).arg(resultat.score, 0, 'f', 1));
    } catch (const std::exception& e) { QMessageBox::warning(this, "Affectation", QString::fromUtf8(e.what())); }
}

void MainWindow::terminerLivraison()
{
    const int ligne = livraisonsTable_->currentRow();
    if (ligne < 0) { QMessageBox::information(this, "Livraison", "Sélectionnez d'abord une livraison « En cours » dans le tableau."); return; }
    const QString statut = enMinuscules(texteDe(livraisonsTable_, ligne, 6));
    if (statut != "en cours" && statut != "en transit") { QMessageBox::information(this, "Livraison", "Seule une livraison « En cours » ou « En transit » peut être terminée.\nStatut actuel : " + texteDe(livraisonsTable_, ligne, 6)); return; }
    auto sansCette = [](QString texte) { texte = texte.trimmed(); if (texte.startsWith('#')) texte.remove(0, 1); return texte; };
    const QString codeLivraison = texteDe(livraisonsTable_, ligne, 0);
    const QString codeCommande  = sansCette(texteDe(livraisonsTable_, ligne, 1));
    const QString codeChauffeur = texteDe(livraisonsTable_, ligne, 3);
    const QString codeVehicule  = texteDe(livraisonsTable_, ligne, 4);
    const auto reponse = QMessageBox::question(this, "Terminer la livraison", QString("Marquer la livraison %1 comme livrée ?\n\nLe véhicule %2 et le chauffeur %3 redeviennent disponibles.").arg(codeLivraison, codeVehicule, codeChauffeur), QMessageBox::Yes | QMessageBox::No);
    if (reponse != QMessageBox::Yes) return;
    auto definir = [](QTableWidget* table, int l, int c, const QString& valeur) { if (QTableWidgetItem* item = table->item(l, c)) item->setText(valeur); else table->setItem(l, c, new QTableWidgetItem(valeur)); };
    definir(livraisonsTable_, ligne, 6, "Livrée");
    for (int r = 0; r < vehiculesTable_->rowCount(); ++r) { if (texteDe(vehiculesTable_, r, 0) == codeVehicule) definir(vehiculesTable_, r, 5, "Disponible"); }
    for (int r = 0; r < chauffeursTable_->rowCount(); ++r) { if (texteDe(chauffeursTable_, r, 0) == codeChauffeur) { definir(chauffeursTable_, r, 5, "Disponible"); definir(chauffeursTable_, r, 4, "Aucun"); } }
    bool commandeTerminee = true;
    for (int r = 0; r < livraisonsTable_->rowCount(); ++r) {
        if (sansCette(texteDe(livraisonsTable_, r, 1)) != codeCommande) continue;
        const QString s = enMinuscules(texteDe(livraisonsTable_, r, 6));
        if (s != "livrée" && s != "livree" && s != "annulée" && s != "annulee") commandeTerminee = false;
    }
    if (commandeTerminee) {
        for (int r = 0; r < commandesTable_->rowCount(); ++r) { if (sansCette(texteDe(commandesTable_, r, 0)) == codeCommande) definir(commandesTable_, r, 3, "Livrée"); }
        for (int r = 0; r < colisTable_->rowCount(); ++r) { if (sansCette(texteDe(colisTable_, r, 1)) == codeCommande) definir(colisTable_, r, 4, "Livré"); }
    }
    QMessageBox::information(this, "Livraison terminée", QString("La livraison %1 est terminée.\n\nVéhicule %2 et chauffeur %3 : de nouveau disponibles.\nCommande %4 : %5").arg(codeLivraison, codeVehicule, codeChauffeur, codeCommande, commandeTerminee ? QString("livrée (colis marqués « Livré »).") : QString("d'autres livraisons restent à effectuer.")));
}

// ================================================================
// COHÉRENCE DES STATUTS ENTRE PAGES
// ================================================================
namespace {
    bool livraisonActive(const QString& statutMinuscules) { return statutMinuscules == "en cours" || statutMinuscules == "en transit" || statutMinuscules == "incident"; }
    QString valeurColonne(const QStringList& ligne, int colonne) { return colonne < ligne.size() ? ligne[colonne].trimmed() : QString(); }
    bool codeUtilisable(const QString& code) { return !code.isEmpty() && enMinuscules(code) != "aucun"; }
    void definirCellule(QTableWidget* table, int ligne, int colonne, const QString& valeur) {
        if (QTableWidgetItem* item = table->item(ligne, colonne)) { if (item->text() != valeur) item->setText(valeur); }
        else table->setItem(ligne, colonne, new QTableWidgetItem(valeur));
    }
}

void MainWindow::appliquerCoherence(QTableWidget* table, int ligne, const QStringList& ancien, const QStringList& nouveau)
{
    if (table == maintenanceTable_) {
        const QString vNouveau = valeurColonne(nouveau, 1); const QString vAncien = valeurColonne(ancien, 1);
        if (codeUtilisable(vNouveau)) reevaluerMaintenance(vNouveau, ligne);
        if (codeUtilisable(vAncien) && vAncien != vNouveau) reevaluerMaintenance(vAncien, -1);
    }
    if (table == incidentsTable_) {
        reevaluerIncidents(valeurColonne(nouveau, 3), valeurColonne(nouveau, 4));
        reevaluerIncidents(valeurColonne(ancien, 3), valeurColonne(ancien, 4));
    }
}

void MainWindow::reevaluerMaintenance(const QString& vehicule, int ligneMaintenance)
{
    bool enCours = false;
    for (int r = 0; r < maintenanceTable_->rowCount(); ++r) { if (texteDe(maintenanceTable_, r, 1) == vehicule && enMinuscules(texteDe(maintenanceTable_, r, 6)) == "en cours") enCours = true; }
    for (int r = 0; r < vehiculesTable_->rowCount(); ++r) {
        if (texteDe(vehiculesTable_, r, 0) != vehicule) continue;
        const QString statut = enMinuscules(texteDe(vehiculesTable_, r, 5));
        if (enCours) {
            if (statut == "en mission") {
                QMessageBox::warning(this, "Maintenance impossible", QString("Le véhicule %1 est en mission.\n\nTerminez d'abord sa livraison : la maintenance est remise au statut « Planifiée ».").arg(vehicule));
                if (ligneMaintenance >= 0) definirCellule(maintenanceTable_, ligneMaintenance, 6, "Planifiée");
            } else { definirCellule(vehiculesTable_, r, 5, "Maintenance"); }
        } else if (statut == "maintenance") {
            bool bloque = false;
            for (int i = 0; i < incidentsTable_->rowCount(); ++i) {
                const QString type = enMinuscules(texteDe(incidentsTable_, i, 2));
                if (texteDe(incidentsTable_, i, 3) == vehicule && enMinuscules(texteDe(incidentsTable_, i, 6)) == "ouvert" && (type == "panne" || type == "accident")) bloque = true;
            }
            bool enLivraison = false;
            for (int i = 0; i < livraisonsTable_->rowCount(); ++i) { if (texteDe(livraisonsTable_, i, 4) == vehicule && livraisonActive(enMinuscules(texteDe(livraisonsTable_, i, 6)))) enLivraison = true; }
            definirCellule(vehiculesTable_, r, 5, bloque ? "Indisponible" : (enLivraison ? "En mission" : "Disponible"));
        }
    }
}

void MainWindow::reevaluerIncidents(const QString& vehicule, const QString& chauffeur)
{
    const bool aVehicule = codeUtilisable(vehicule); const bool aChauffeur = codeUtilisable(chauffeur);
    if (!aVehicule && !aChauffeur) return;
    auto incidentOuvert = [this](const QString& v, const QString& c, bool bloquantSeulement) {
        for (int r = 0; r < incidentsTable_->rowCount(); ++r) {
            if (enMinuscules(texteDe(incidentsTable_, r, 6)) != "ouvert") continue;
            const QString type = enMinuscules(texteDe(incidentsTable_, r, 2));
            if (bloquantSeulement && type != "panne" && type != "accident") continue;
            const bool memeVehicule = codeUtilisable(v) && texteDe(incidentsTable_, r, 3) == v;
            const bool memeChauffeur = !bloquantSeulement && codeUtilisable(c) && texteDe(incidentsTable_, r, 4) == c;
            if (memeVehicule || memeChauffeur) return true;
        }
        return false;
    };
    for (int r = 0; r < livraisonsTable_->rowCount(); ++r) {
        const QString v = texteDe(livraisonsTable_, r, 4); const QString c = texteDe(livraisonsTable_, r, 3);
        const bool concernee = (aVehicule && v == vehicule) || (aChauffeur && c == chauffeur);
        const QString statut = enMinuscules(texteDe(livraisonsTable_, r, 6));
        if (!concernee || !livraisonActive(statut)) continue;
        const bool incident = incidentOuvert(v, c, false);
        QString commandeCode = texteDe(livraisonsTable_, r, 1); if (commandeCode.startsWith('#')) commandeCode.remove(0, 1);
        if (incident && statut != "incident") {
            definirCellule(livraisonsTable_, r, 6, "Incident");
            for (int k = 0; k < commandesTable_->rowCount(); ++k) { QString code = texteDe(commandesTable_, k, 0); if (code.startsWith('#')) code.remove(0, 1); if (code == commandeCode) definirCellule(commandesTable_, k, 3, "Incident"); }
        } else if (!incident && statut == "incident") {
            definirCellule(livraisonsTable_, r, 6, "En cours");
            for (int k = 0; k < commandesTable_->rowCount(); ++k) { QString code = texteDe(commandesTable_, k, 0); if (code.startsWith('#')) code.remove(0, 1); if (code == commandeCode && enMinuscules(texteDe(commandesTable_, k, 3)) == "incident") definirCellule(commandesTable_, k, 3, "En cours"); }
        }
    }
    if (aVehicule) {
        const bool bloque = incidentOuvert(vehicule, QString(), true);
        for (int r = 0; r < vehiculesTable_->rowCount(); ++r) {
            if (texteDe(vehiculesTable_, r, 0) != vehicule) continue;
            const QString statut = enMinuscules(texteDe(vehiculesTable_, r, 5));
            if (bloque && statut != "maintenance" && statut != "indisponible") { definirCellule(vehiculesTable_, r, 5, "Indisponible"); }
            else if (!bloque && statut == "indisponible") {
                bool enLivraison = false;
                for (int i = 0; i < livraisonsTable_->rowCount(); ++i) { if (texteDe(livraisonsTable_, i, 4) == vehicule && livraisonActive(enMinuscules(texteDe(livraisonsTable_, i, 6)))) enLivraison = true; }
                definirCellule(vehiculesTable_, r, 5, enLivraison ? "En mission" : "Disponible");
            }
        }
    }
}

// ================================================================
// ITINÉRAIRES : Dijkstra + stratégies
// ================================================================
void MainWindow::calculerItineraire()
{
    const int ligne = itinerairesTable_->currentRow();
    if (ligne < 0) { QMessageBox::information(this, "Itinéraire", "Sélectionnez d'abord un itinéraire dans le tableau."); return; }
    const QString depart = texteDe(itinerairesTable_, ligne, 1); const QString destination = texteDe(itinerairesTable_, ligne, 2);
    if (depart == destination) { QMessageBox::information(this, "Itinéraire", "Le départ et la destination sont identiques."); return; }
    sl::Graphe graphe; sl::construireReseauMalgache(graphe);
    for (const QString& ville : {depart, destination}) {
        if (!graphe.contientVille(ville.toStdString())) { QMessageBox::warning(this, "Itinéraire", QString("« %1 » ne fait pas partie du réseau routier.\n\nVilles disponibles :\n%2").arg(ville, villesDuReseau().join(", "))); return; }
    }
    bool ok = false;
    const QStringList criteres = {"Plus court (distance)", "Plus rapide (durée)", "Moins cher (coût)"};
    const QString critere = QInputDialog::getItem(this, "Calcul d'itinéraire", QString("%1 → %2\n\nCritère d'optimisation :").arg(depart, destination), criteres, 0, false, &ok);
    if (!ok) return;

    const sl::StrategiePlusCourtChemin strategieCourte;
    const sl::StrategiePlusRapide strategieRapide;
    const sl::StrategieMoinsCher strategieEconomique;
    const sl::StrategieItineraire* strategie = &strategieCourte;
    if (critere == criteres[1]) strategie = &strategieRapide;
    if (critere == criteres[2]) strategie = &strategieEconomique;

    const sl::Itineraire itineraire = sl::calculerPlusCourtChemin(graphe, depart.toStdString(), destination.toStdString(), *strategie);
    if (!itineraire.trouve) { QMessageBox::warning(this, "Itinéraire", QString("Aucun itinéraire praticable entre %1 et %2 (routes bloquées ?).").arg(depart, destination)); return; }

    auto groupe = [](long long valeur) {
        const QString chiffres = QString::number(valeur); QString resultat; int n = 0;
        for (int i = chiffres.size() - 1; i >= 0; --i) { if (n > 0 && n % 3 == 0) resultat.prepend(' '); resultat.prepend(chiffres[i]); ++n; } return resultat;
    };

    const int minutes = qRound(itineraire.dureeTotaleHeures * 60.0);
    QString duree;
    if (minutes < 60) duree = QString("%1 min").arg(minutes);
    else if (minutes % 60 == 0) duree = QString("%1h").arg(minutes / 60);
    else duree = QString("%1h %2").arg(minutes / 60).arg(minutes % 60, 2, 10, QChar('0'));

    QStringList villes;
    for (const std::string& ville : itineraire.villes) villes << QString::fromStdString(ville);
    const QString trajet = villes.join(" → ");
    const QString distance = groupe(qRound64(itineraire.distanceTotaleKm)) + " km";

    auto definir = [this, ligne](int colonne, const QString& valeur) {
        if (QTableWidgetItem* item = itinerairesTable_->item(ligne, colonne)) item->setText(valeur);
        else itinerairesTable_->setItem(ligne, colonne, new QTableWidgetItem(valeur));
    };

    definir(3, distance);
    definir(4, duree);
    definir(5, "Optimisé");
    definir(6, trajet);

    QMessageBox::information(this, "Itinéraire calculé", QString("%1\n\nTrajet : %2\nDistance : %3\nDurée : %4\nCoût estimé : %5 Ar").arg(critere, trajet, distance, duree).arg(groupe(qRound64(itineraire.coutTotalAr))));
}

// ================================================================
// ALERTES DE MAINTENANCE
// ================================================================
void MainWindow::verifierAlertesMaintenance()
{
    if (!vehiculesTable_ || !maintenanceTable_) return;

    QStringList alertes;
    for (int v = 0; v < vehiculesTable_->rowCount(); ++v) {
        QString codeVehicule = texteDe(vehiculesTable_, v, 0);
        QString statut = enMinuscules(texteDe(vehiculesTable_, v, 5));
        QString kilometrage = texteDe(vehiculesTable_, v, 6);
        double km = nombreDe(kilometrage);

        bool maintenanceEnCours = false;
        QString typeMaintenance;
        for (int m = 0; m < maintenanceTable_->rowCount(); ++m) {
            if (texteDe(maintenanceTable_, m, 1) == codeVehicule && enMinuscules(texteDe(maintenanceTable_, m, 6)) == "en cours") {
                maintenanceEnCours = true;
                typeMaintenance = texteDe(maintenanceTable_, m, 2);
                break;
            }
        }

        if (maintenanceEnCours) alertes << QString("🔧 %1 en maintenance (%2)").arg(codeVehicule, typeMaintenance);
        if (km >= 100000 && statut != "maintenance") alertes << QString("⚠️ %1 : kilométrage élevé (%2 km)").arg(codeVehicule).arg(groupeParTrois(qRound64(km)));
        if (statut == "indisponible") alertes << QString("🚫 %1 : véhicule indisponible").arg(codeVehicule);
    }

    QWidget* tableauDeBord = pages_->widget(0);
    QLabel* alerteLabel = tableauDeBord->findChild<QLabel*>("alertesMaintenance");

    if (!alerteLabel) {
        QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(tableauDeBord->layout());
        if (layout) {
            alerteLabel = new QLabel();
            alerteLabel->setObjectName("alertesMaintenance");
            alerteLabel->setWordWrap(true);
            layout->insertWidget(1, alerteLabel);
        }
    }

    if (alerteLabel) {
        if (alertes.isEmpty()) {
            alerteLabel->setText("✅ Aucune alerte de maintenance");
            alerteLabel->setStyleSheet("background-color: #d1fae5; color: #065f46; padding: 15px; border-radius: 8px; font-weight: 600;");
        } else {
            alerteLabel->setText("⚠️ Alertes de maintenance :\n\n" + alertes.join("\n"));
            alerteLabel->setStyleSheet("background-color: #fef3c7; color: #92400e; padding: 15px; border-radius: 8px; font-weight: 600;");
        }
    }
}