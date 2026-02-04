#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QList>
#include "alumni.h"
#include "databasehandler.h"
#include <QListWidgetItem>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateDonations();
    void onSearchButtonClicked();
    void onAdminButtonClicked();
    void onAdminLoginSuccess();
    void onAdminLoginFailed();
    void onProfileSelected(QListWidgetItem *item);
    void changeImages();

private:
    Ui::MainWindow *ui;
    QTimer *donationTimer;
    double donationAmount;
    QList<Alumni> alumniList;
    QTimer *imageTimer;
    QList<QString> imagePaths;
    int currentImageIndex;
    DatabaseHandler *dbHandler;

    // Add these declarations to match your implementation
    bool initializeDatabase();
    void initializeUI();
    void setupImageSlideshow();
    void loadLogoImages();
    void setupDonationSystem();
    void setupConnections();
    void refreshAlumniList();
};

#endif // MAINWINDOW_H
