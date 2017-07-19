#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QAction>
#include <QMenu>
#include <QObject>
#include <QSystemTrayIcon>

class GUI;
class Settings;

class SystemTray : public QObject
{
    Q_OBJECT
    public:
        explicit SystemTray(GUI *parent = nullptr);
        ~SystemTray();

        //external
        GUI *gui;
        Settings *settings;

        QMenu *trayMenu;
        QSystemTrayIcon *trayIcon;
        QAction *quitAction;
        QAction *settingsAction;
    private:

    private slots:
        void createTrayIcon();
        void createContextMenu();
        void trayActivate(QSystemTrayIcon::ActivationReason reason);
        void openSettings();
        void exit();
};

#endif // SYSTEMTRAY_H
