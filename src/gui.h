#ifndef GUI_H
#define GUI_H

#include <QHotkey>
#include <QObject>
#include <QSettings>
#include <QTimer>

class Account;
class Settings;
class Skypush;
class SystemTray;

class GUI : public QObject
{
    Q_OBJECT

    public:
        explicit GUI(Skypush *parent = nullptr);

        Account *account;
        Settings *settings;
        Skypush *skypush;
        SystemTray *systemTray;

        QSettings *settingsManager;
        QHotkey *areaHotkey;
        QHotkey *windowHotkey;
        QHotkey *everythingHotkey;
        bool privateUpload;
        QString clientToken;
        QString sessionToken;
        QString accessToken;
        QTimer* accessTokenRefreshTimer;

    public slots:
        bool registerHotkeys();
        bool registerAreaHotkey();
        bool registerWindowHotkey();
        bool registerEverythingHotkey();
        void setClientToken();
        void getClientToken();
        void getClientTokenReply();
        void setAccessToken();
        void getAccessToken();
        void getAccessTokenReply();
        void refreshAccessToken();
        void refreshAccessTokenReply();
};

#endif // GUI_H
