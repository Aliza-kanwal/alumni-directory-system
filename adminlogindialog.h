#ifndef ADMINLOGINDIALOG_H
#define ADMINLOGINDIALOG_H


#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class AdminLoginDialog; }
QT_END_NAMESPACE

class AdminLoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AdminLoginDialog(QWidget *parent = nullptr);
    ~AdminLoginDialog();

signals:
    void loginSuccess(); // Signal emitted on successful login
    void loginFailed();  // Signal emitted on failed login

private slots:
    void onLoginButtonClicked(); // Slot for login button click


private:
    Ui::AdminLoginDialog *ui;
    const QString ADMIN_USERNAME = "admin@neduet.edu.pk";
    const QString ADMIN_PASSWORD = "1234";
};

#endif // ADMINLOGINDIALOG_H
