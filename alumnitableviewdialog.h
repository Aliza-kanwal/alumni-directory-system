#ifndef ALUMNITABLEVIEWDIALOG_H
#define ALUMNITABLEVIEWDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include "databasehandler.h"

namespace Ui {
class AlumniTableViewDialog;
}

class AlumniTableViewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AlumniTableViewDialog(DatabaseHandler *dbHandler, QWidget *parent = nullptr);
    ~AlumniTableViewDialog();

private:
    Ui::AlumniTableViewDialog *ui;
    DatabaseHandler *dbHandler;
    QStandardItemModel *model;

    void setupTable();
    void loadData();
};

#endif // ALUMNITABLEVIEWDIALOG_H
