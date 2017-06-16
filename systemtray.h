#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

class SystemTray : public QObject
{
    Q_OBJECT
    public:
        explicit SystemTray(QObject *parent = nullptr);
        ~SystemTray();

        QMenu *trayMenu;
        QSystemTrayIcon *trayIcon;
        QAction *quitAction;
        QAction *settingsAction;

    private:

    private slots:
        void trayActivate(QSystemTrayIcon::ActivationReason reason);
        void openSettings();
        void createContextMenu();
        void createTrayIcon();
        void exit();
};

#endif // SYSTEMTRAY_H
