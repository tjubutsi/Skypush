#ifndef GUI_H
#define GUI_H

#include <QHotkey>
#include <QObject>
#include <QSettings>

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
};

#endif // GUI_H
