#include "trayhandler.h"
#include <QSystemTrayIcon>
#include <QDebug>

TrayHandler::TrayHandler(QObject *parent) : QObject(parent)
{
    trayIcon = new QSystemTrayIcon();
    if (trayIcon->isSystemTrayAvailable()) //TODO: settings verification too
    {
        trayIcon->setIcon(QIcon("://icon.png"));
        trayIcon->show();
    }

    connect(trayIcon, &QSystemTrayIcon::activated, this, &TrayHandler::test);
}

void TrayHandler::test(QSystemTrayIcon::ActivationReason reason)
{
    //TODO: context menu
}
