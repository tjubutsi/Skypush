#ifndef TRAYHANDLER_H
#define TRAYHANDLER_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

class TrayHandler : public QObject
{
    Q_OBJECT
    public:
        explicit TrayHandler(QObject *parent = nullptr);

    private:
        QMenu *trayMenu;
        QSystemTrayIcon *trayIcon;
        QAction *quitAction;
        QAction *settingsAction;

    private slots:
        void trayActivate(QSystemTrayIcon::ActivationReason reason);
        void openSettings();
};

#endif // TRAYHANDLER_H
