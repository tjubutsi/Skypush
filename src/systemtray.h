#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QAction>
#include <QMenu>
#include <QObject>
#include <QSystemTrayIcon>

class GUI;

class SystemTray : public QObject
{
    Q_OBJECT
    public:
        explicit SystemTray(GUI *parent = nullptr);
        ~SystemTray();

        //external
        GUI *gui;

        QMenu *trayMenu;
        QSystemTrayIcon *trayIcon;
        QAction *settingsAction;
        QAction *accountAction;
        QAction *quitAction;

    private:

    private slots:
        void createTrayIcon();
        void createContextMenu();
        void trayActivate(QSystemTrayIcon::ActivationReason reason);
        void openSettings();
        void openAccount();
        void exit();
};

#endif // SYSTEMTRAY_H
