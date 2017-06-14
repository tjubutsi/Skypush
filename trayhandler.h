#ifndef TRAYHANDLER_H
#define TRAYHANDLER_H

#include <QObject>
#include <QSystemTrayIcon>

class TrayHandler : public QObject
{
    Q_OBJECT
    public:
        explicit TrayHandler(QObject *parent = nullptr);

    private:
        QSystemTrayIcon *trayIcon;

    private slots:
        void test(QSystemTrayIcon::ActivationReason reason);
};

#endif // TRAYHANDLER_H
