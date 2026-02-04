#ifndef ADMINDASHBOARDDIALOG_H
#define ADMINDASHBOARDDIALOG_H

#include <QDialog>
#include <QList>
#include "alumni.h"

// Forward declaration to avoid circular includes
class DatabaseHandler;

namespace Ui {
class AdminDashboardDialog;
}

class AdminDashboardDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AdminDashboardDialog(DatabaseHandler *dbHandler, QWidget *parent = nullptr);
    ~AdminDashboardDialog();

    void setDatabaseHandler(DatabaseHandler *handler);  // Proper declaration
    void refreshAlumniList();

signals:
    void dataUpdated();

private slots:
    void on_addAlumniButton_clicked();
    void on_updateInfoButton_clicked();
    void on_showDataButton_clicked();

private:
    Ui::AdminDashboardDialog *ui;
    DatabaseHandler *dbHandler;
    QList<Alumni> alumniList;
};

#endif // ADMINDASHBOARDDIALOG_H
