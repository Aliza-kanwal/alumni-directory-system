#include "updateinfodialog.h"
#include "ui_updateinfodialog.h"
#include <QMessageBox>
#include <QDebug>
#include "databasehandler.h"

UpdateInfoDialog::UpdateInfoDialog(DatabaseHandler *dbHandler, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateInfoDialog),
    dbHandler(dbHandler)
{
    ui->setupUi(this);

    // Connect UI elements
    connect(ui->pushButton, &QPushButton::clicked, this, &UpdateInfoDialog::onSearchButtonClicked);

    // You'll need to add update and delete buttons in your UI and connect them

    // For example:
    connect(ui->updateButton, &QPushButton::clicked, this, &UpdateInfoDialog::onUpdateButtonClicked);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &UpdateInfoDialog::onDeleteButtonClicked);
    // Using Qt Resource System (recommended)
    // In your dialog/window constructor or setup function:
    QPixmap image(":/images/nedlogo.jpg"); // Replace with your actual path
    if (image.isNull()) {
        qDebug() << "Error: Image not loaded! Check path:" << ":/images/ned_logo.png";
    } else {
        ui->label_2->setPixmap(image);
        qDebug() << "Image loaded successfully.";
    }
}

UpdateInfoDialog::~UpdateInfoDialog()
{
    delete ui;
}

void UpdateInfoDialog::setAlumniList(const QList<Alumni> &list)
{
    alumniList = list;
}

QList<Alumni> UpdateInfoDialog::getUpdatedAlumniList() const
{
    return alumniList;
}

void UpdateInfoDialog::onSearchButtonClicked()
{
    QString searchText = ui->lineEdit->text().trimmed();
    if (searchText.isEmpty()) {
        QMessageBox::warning(this, "Search Error", "Please enter a name to search");
        return;
    }

    bool found = false;
    for (int i = 0; i < alumniList.size(); ++i) {
        if (alumniList[i].getName().contains(searchText, Qt::CaseInsensitive)) {
            currentIndex = i;
            populateFields(alumniList[i]);
            found = true;
            break;
        }
    }

    if (!found) {
        QMessageBox::information(this, "Not Found", "No alumni found with that name");
    }
}

void UpdateInfoDialog::populateFields(const Alumni &alumni)
{
    ui->nameLineEdit->setText(alumni.getName());
    //ui->deptLineEdit_2->setText(alumni.getDepartment());
    //ui->emailLineEdit->setText(alumni.getEmail());
    ui->fieldLineEdit->setText(alumni.getField());
    ui->linkedinLineEdit->setText(alumni.getLinkedIn());
    ui->yearLineEdit->setText(alumni.getGraduationYear());
    ui->degreeLineEdit->setText(alumni.getDegree());
}

void UpdateInfoDialog::onUpdateButtonClicked() {
    if (currentIndex < 0) {
        QMessageBox::warning(this, "Error", "No alumni selected");
        return;
    }

    // Get original alumni data
    const Alumni& original = alumniList[currentIndex];

    // Get new values from UI
    QString newName = ui->nameLineEdit->text().trimmed();
    QString newYear = ui->yearLineEdit->text().trimmed();
    QString newLinkedIn = ui->linkedinLineEdit->text().trimmed();
    QString newField = ui->fieldLineEdit->text().trimmed();
    QString newDegree = ui->degreeLineEdit->text().trimmed();

    // Validate critical fields
    if (newName.isEmpty()) {
        QMessageBox::warning(this, "Error", "Name cannot be empty");
        return;
    }

    // Confirm update
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirm Update",
        QString("Update alumni record?\n\nOriginal Name: %1\nNew Name: %2")
            .arg(original.getName()).arg(newName),
        QMessageBox::Yes|QMessageBox::No);

    if (reply != QMessageBox::Yes) {
        return;
    }

    // Make a copy of the alumni data
    Alumni updated = original;
    updated.setName(newName);
    updated.setGraduationYear(newYear);
    updated.setLinkedIn(newLinkedIn);
    updated.setField(newField);
    updated.setDegree(newDegree);

    // Update database
    if (dbHandler && dbHandler->updateAlumni(original.getName(), updated.toMap())) {
        // Only update local list if DB update succeeded
        alumniList[currentIndex] = updated;
        emit alumniUpdated(alumniList);
        QMessageBox::information(this, "Success", "Update successful");
    } else {
        QMessageBox::critical(this, "Error", "Update failed - database error");
    }
}
void UpdateInfoDialog::onDeleteButtonClicked()
{
    // 1. Validate UI state
    if (!ui || !dbHandler) {
        qCritical() << "UI or DB handler not initialized";
        QMessageBox::critical(this, "Error", "System error: Invalid initialization");
        return;
    }

    // 2. Validate selection
    if (currentIndex < 0 || currentIndex >= alumniList.size()) {
        QMessageBox::warning(this, "Error", "Please select an alumni record first");
        return;
    }

    // 3. Get alumni data safely
    const Alumni &alumni = alumniList.at(currentIndex); // Using at() for bounds checking
    QString alumniName = alumni.getName();
    if (alumniName.isEmpty()) {
        QMessageBox::warning(this, "Error", "Selected alumni has no valid name");
        return;
    }

    // 4. Confirm deletion
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm Delete",
                                  QString("Delete alumni:\n\nName: %1\nYear: %2\nDepartment: %3")
                                      .arg(alumniName)
                                      .arg(alumni.getGraduationYear())
                                      .arg(alumni.getDepartment()),
                                  QMessageBox::Yes|QMessageBox::No);

    if (reply != QMessageBox::Yes) {
        return;
    }

    // 5. Execute deletion with error handling
    try {
        bool success = false;
        {
            QApplication::setOverrideCursor(Qt::WaitCursor);
            success = dbHandler->deleteAlumni(alumniName);
            QApplication::restoreOverrideCursor();
        }

        if (success) {
            // Update local model
            alumniList.removeAt(currentIndex);

            // Update UI
            populateFields(Alumni());
            currentIndex = -1;
            ui->yearLineEdit->clear();

            // Notify listeners
            emit alumniUpdated(alumniList);

            QMessageBox::information(this, "Success", "Alumni deleted successfully");
        } else {
            QMessageBox::critical(this, "Error",
                                  QString("Failed to delete alumni:\n%1")
                                      .arg(dbHandler->lastError()));
        }
    } catch (const std::exception &e) {
        QApplication::restoreOverrideCursor();
        qCritical() << "Exception during deletion:" << e.what();
        QMessageBox::critical(this, "Error",
                              QString("Critical error:\n%1").arg(e.what()));
    } catch (...) {
        QApplication::restoreOverrideCursor();
        qCritical() << "Unknown exception during deletion";
        QMessageBox::critical(this, "Error", "Unknown critical error occurred");
    }
}
