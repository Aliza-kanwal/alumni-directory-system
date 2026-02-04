#include "adminlogindialog.h"
#include "ui_adminlogindialog.h"
#include <QMessageBox>
#include <QPixmap>
#include <QDebug>

AdminLoginDialog::AdminLoginDialog(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::AdminLoginDialog)
{
    ui->setupUi(this);

    // Password masking
    ui->passwordLineEdit_3->setEchoMode(QLineEdit::Password);

        QPixmap image(":/images/background.jpeg");
        if (!image.isNull()) {
            ui->label_2->setPixmap(image.scaled(ui->label_2->size(),
                                                Qt::KeepAspectRatio,
                                                Qt::SmoothTransformation));
        } else {
            qWarning() << "Failed to load logo image";
        }

    // Load endtag image
    QPixmap pixmap4(":/images/endtag.jpeg");
    if (pixmap4.isNull()) {
        qDebug() << "Failed to load endtag.jpeg!";
    } else {
        qDebug() << "endtag.jpeg loaded successfully!";
        ui->label_41->setPixmap(pixmap4);
    }

    // Connect login button
    connect(ui->loginButton_3, &QPushButton::clicked, this, &AdminLoginDialog::onLoginButtonClicked);

    QPixmap pixmap3(":/images/nedlogo.jpg");
    if (!pixmap3.isNull()) {
        ui->label_3->setPixmap(pixmap3);  // Corrected to use pixmap4
        ui->label_3->setScaledContents(true);  // Ensure image scales to label size
    } else {
        qDebug() << "Failed to load map2.jpg";
    }

}

AdminLoginDialog::~AdminLoginDialog()
{
    delete ui;
}
void AdminLoginDialog::onLoginButtonClicked() {
    QString username = ui->usernameLineEdit_3->text().trimmed();
    QString password = ui->passwordLineEdit_3->text().trimmed();

    // Case-sensitive comparison
    if (username.compare(ADMIN_USERNAME, Qt::CaseSensitive) == 0 &&
        password.compare(ADMIN_PASSWORD, Qt::CaseSensitive) == 0) {
        emit loginSuccess();
        close();
    } else {
        emit loginFailed();
        QMessageBox::warning(this, "Login Failed", "Invalid username or password.");
    }
}
