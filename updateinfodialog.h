#ifndef UPDATEINFODIALOG_H
#define UPDATEINFODIALOG_H

#include <QDialog>
#include <QList>
#include "alumni.h"
#include "databasehandler.h"
class DatabaseHandler;
namespace Ui {
class UpdateInfoDialog;
}

class UpdateInfoDialog : public QDialog
{
    Q_OBJECT

public:

    explicit UpdateInfoDialog(DatabaseHandler *dbHandler, QWidget *parent = nullptr);
    ~UpdateInfoDialog();

    void setAlumniList(const QList<Alumni> &list);
    QList<Alumni> getUpdatedAlumniList() const;

signals:
    void alumniUpdated(const QList<Alumni> &updatedList);

private slots:
    void onSearchButtonClicked();
    void onUpdateButtonClicked();
    void onDeleteButtonClicked();
    void populateFields(const Alumni &alumni);

private:
    Ui::UpdateInfoDialog *ui;
    QList<Alumni> alumniList;
    int currentIndex = -1;
    DatabaseHandler *dbHandler;
};

#endif // UPDATEINFODIALOG_H
