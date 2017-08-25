#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QSettings>

class GUI;

namespace Ui {
    class Account;
}

class Account : public QDialog
{
    Q_OBJECT

    public:
        explicit Account(GUI *parent = nullptr);
        ~Account();

        GUI *gui;

        QNetworkAccessManager *networkManager;
        QSettings *settingsManager;

    private slots:
        void initialize();
        void enableLogin(bool state);
        void on_existingUserToggle_stateChanged(int arg1);
        void on_loginButtonBox_accepted();
        void on_loginButtonBox_rejected();
        void on_loggedInButtonBox_accepted();
        void on_loggedInButtonBox_rejected();
        void createUser();
        void createUserReply();
        void login();
        void loginReply();

    private:
        Ui::Account *ui;
};

#endif // ACCOUNT_H
