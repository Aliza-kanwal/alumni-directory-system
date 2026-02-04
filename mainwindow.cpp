#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "adminlogindialog.h"
#include "alumni.h"
#include "searchresultdialog.h"
#include <QListWidgetItem>
#include <QMessageBox>
#include "admindashboarddialog.h"
#include <QPushButton>
#include <QToolButton>
#include "customlistwidget.h"
#include <QDebug>
#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    donationAmount(0.0),
    currentImageIndex(0),
    dbHandler(new DatabaseHandler(this))
{
    ui->setupUi(this);

    // Initialize database
    if (!initializeDatabase()) {
        QMessageBox::critical(this, "Database Error",
                              "Failed to initialize database. Some features may be limited.");
    }
    else {
        qDebug() << "Database initialized successfully. Records loaded:"
                 << dbHandler->getAllAlumni().size();
    }

    // Initialize UI components
    initializeUI();

    // Load initial data
    refreshAlumniList();
}

MainWindow::~MainWindow()
{
    // Clean up timers
    delete donationTimer;
    delete imageTimer;

    // Clean up UI
    delete ui;

    // Clean up database handler
    delete dbHandler;
}

void MainWindow::initializeUI()
{
    // Image slideshow setup
    setupImageSlideshow();

    // Initialize search filters
    ui->comboBox->addItem("Name");
    ui->comboBox->addItem("Department");
    ui->comboBox->addItem("Graduation Year");

    // Load logo images
    loadLogoImages();

    // Donation system
    setupDonationSystem();

    // Initial UI state
    ui->searchButton->hide();
    ui->lineEdit->hide();
    ui->listWidget->hide();
    ui->widget_3->setVisible(!ui->widget_3->isVisible());

    // Connect signals and slots
    setupConnections();
}

bool MainWindow::initializeDatabase()
{
    // Try to open from resources first
    if (QFile::exists(":/database/Alumni_Data.db")) {
        QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QDir dir(appDataPath);

        // Create directory if it doesn't exist
        if (!dir.exists() && !dir.mkpath(appDataPath)) {
            qDebug() << "Failed to create application data directory";
            return false;
        }

        QString dbPath = appDataPath + "/Alumni_Data.db";

        // Copy database if needed
        if (!QFile::exists(dbPath) ||
            (QFileInfo(":/database/Alumni_Data.db").lastModified() > QFileInfo(dbPath).lastModified())) {
            if (QFile::exists(dbPath) && !QFile::remove(dbPath)) {
                qDebug() << "Failed to remove old database file";
                return false;
            }

            if (!QFile::copy(":/database/Alumni_Data.db", dbPath)) {
                qDebug() << "Failed to copy database to writable location";
                return false;
            }

            // Set proper permissions
            QFile::setPermissions(dbPath, QFile::WriteOwner | QFile::ReadOwner | QFile::ReadGroup | QFile::ReadOther);
        }

        // Open the database
        if (!dbHandler->openDatabase(dbPath)) {
            qDebug() << "Failed to open database:" << dbHandler->lastError();
            return false;
        }

        return true;
    }

    qDebug() << "Database resource not found";
    return false;
}

void MainWindow::setupImageSlideshow()
{
    imagePaths << ":/images/image-1-.png"
               << ":/images/image4.png"
               << ":/images/image3.jpeg"
               << ":/images/image2.jpeg"
               << ":/images/image3.jpeg";

    if (!imagePaths.isEmpty()) {
        ui->imagelabel1->setPixmap(QPixmap(imagePaths.first()));
    }

    imageTimer = new QTimer(this);
    connect(imageTimer, &QTimer::timeout, this, &MainWindow::changeImages);
    imageTimer->start(5000); // Change image every 5 seconds
}

void MainWindow::loadLogoImages()
{
    const QMap<QString, QLabel*> logoMap = {
        {":/images/grey.jpeg", ui->label_4},
        {":/images/orange.jpg", ui->label_5},
        {":/images/image.png", ui->label_9},
        {":/images/map2.jpg", ui->label}
    };

    for (auto it = logoMap.begin(); it != logoMap.end(); ++it) {
        QPixmap pixmap(it.key());
        if (!pixmap.isNull()) {
            it.value()->setPixmap(pixmap);
            if (it.value() == ui->label) {
                ui->label->setScaledContents(true);
            }
        } else {
            qDebug() << "Failed to load image:" << it.key();
        }
    }
}

void MainWindow::setupDonationSystem()
{
    ui->label_8->setText(QString("Donations: %1$").arg(donationAmount));
    donationTimer = new QTimer(this);
    connect(donationTimer, &QTimer::timeout, this, &MainWindow::updateDonations);
    donationTimer->start(30000); // Update every 30 seconds
}

void MainWindow::setupConnections()
{
    // Button connections
    connect(ui->adminButton, &QPushButton::clicked, this, &MainWindow::onAdminButtonClicked);
    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::onSearchButtonClicked);

    // Tool button connections
    connect(ui->toolButton, &QToolButton::clicked, this, [this]() {
        bool visible = !ui->lineEdit->isVisible();
        ui->lineEdit->setVisible(visible);
        ui->searchButton->setVisible(visible);
        ui->comboBox->setVisible(visible);
        ui->listWidget->setVisible(visible);
        if (visible) ui->lineEdit->setFocus();
    });

    connect(ui->toolButton_2, &QPushButton::clicked, this, [this]() {
        ui->label_10->setVisible(!ui->label_10->isVisible());
        ui->widget_3->setVisible(!ui->widget_3->isVisible());
    });

    connect(ui->toolButton_3, &QPushButton::clicked, this, [this]() {
        bool newVisibility = !ui->widget_6->isVisible();
        ui->widget_6->setVisible(newVisibility);
        ui->widget_7->setVisible(newVisibility);
        ui->label->setVisible(newVisibility);
        ui->label_11->setVisible(newVisibility);
    });

    // List widget connection
    connect(ui->listWidget, &QListWidget::itemClicked, this, &MainWindow::onProfileSelected);
}

void MainWindow::refreshAlumniList()
{
    // Clear existing data
    alumniList.clear();
    ui->listWidget->clear();

    if (!dbHandler || !dbHandler->isOpen()) {
        qDebug() << "Database not available for refresh";
        return;
    }

    // Get data from database
    auto alumniData = dbHandler->getAllAlumni();
    if (alumniData.isEmpty()) {
        qDebug() << "No alumni records found in database";
        return;
    }

    // Process and display data
    for (const auto &data : alumniData) {
        Alumni alumni(
            data["name"].toString(),
            "", // department - not in database
            data["graduation_year"].toString(),
            "", // email - not in database
            data["linkedin_profile"].toString(),
            data["main_skills"].toString(), // using as field
            data["qualifications"].toString() // using as degree
            );

        alumniList.append(alumni);
        ui->listWidget->addItem(alumni.getName());
    }

    qDebug() << "Refreshed alumni list. Total records:" << alumniList.size();
}

void MainWindow::updateDonations()
{
    donationAmount += 100.0;
    ui->label_8->setText(QString("Donations: %1$").arg(donationAmount));
    qDebug() << "Donation updated. Current amount:" << donationAmount;
}

void MainWindow::onSearchButtonClicked()
{
    QString searchText = ui->lineEdit->text().trimmed();
    if (searchText.isEmpty()) {
        refreshAlumniList();
        return;
    }

    QString filter = ui->comboBox->currentText();
    ui->listWidget->clear();

    bool foundResults = false;
    for (const Alumni &alumni : alumniList) {
        bool match = false;

        if (filter == "Name") {
            match = alumni.getName().contains(searchText, Qt::CaseInsensitive);
        }
        else if (filter == "Department") {
            match = alumni.getDepartment().contains(searchText, Qt::CaseInsensitive);
        }
        else if (filter == "Graduation Year") {
            match = alumni.getGraduationYear() == searchText;
        }

        if (match) {
            ui->listWidget->addItem(alumni.getName());
            foundResults = true;
        }
    }

    ui->listWidget->setVisible(foundResults);
    if (!foundResults) {
        ui->statusbar->showMessage("No matching records found", 3000);
    }
}

void MainWindow::onAdminButtonClicked()
{
    AdminLoginDialog adminDialog;
    connect(&adminDialog, &AdminLoginDialog::loginSuccess, this, &MainWindow::onAdminLoginSuccess);
    connect(&adminDialog, &AdminLoginDialog::loginFailed, this, &MainWindow::onAdminLoginFailed);

    adminDialog.exec();
}

void MainWindow::onAdminLoginSuccess()
{
    AdminDashboardDialog *adminDashboard = new AdminDashboardDialog(dbHandler, this);

    connect(adminDashboard, &AdminDashboardDialog::dataUpdated,
            this, &MainWindow::refreshAlumniList);

    adminDashboard->exec();
    delete adminDashboard;
}

// In onAdminLoginFailed():
void MainWindow::onAdminLoginFailed()
{
    QMessageBox::warning(this, "Login Failed", "Invalid credentials. Access denied.");
    ui->statusbar->showMessage("Admin login failed", 3000);  // Fixed statusbar
}
void MainWindow::onProfileSelected(QListWidgetItem *item)
{
    if (!item) return;

    QString selectedName = item->text();
    auto it = std::find_if(alumniList.begin(), alumniList.end(),
                           [&selectedName](const Alumni &a) {
                               return a.getName() == selectedName;
                           });

    if (it != alumniList.end()) {
        SearchResultDialog resultDialog(this);
        resultDialog.setSearchResult(
            it->getName(),
            //it->getDepartment(),
            it->getGraduationYear(),
            //it->getEmail(),
            it->getLinkedIn(),
            it->getField(),
            it->getDegree()
            );

        resultDialog.exec();
    }
}

void MainWindow::changeImages()
{
    if (imagePaths.isEmpty()) return;

    currentImageIndex = (currentImageIndex + 1) % imagePaths.size();
    QPixmap pixmap(imagePaths[currentImageIndex]);

    if (!pixmap.isNull()) {
        ui->imagelabel1->setPixmap(pixmap);
    }
}
