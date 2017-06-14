#include "trayhandler.h"
#include <QApplication>
#include <QSystemTrayIcon>
#include <QDebug>
#include <QMenu>
#include <qlabel.h>

TrayHandler::TrayHandler(QObject *parent) : QObject(parent)
{
    trayIcon = new QSystemTrayIcon();
    if (trayIcon->isSystemTrayAvailable()) //TODO: settings verification too
    {
        quitAction = new QAction(tr("&Exit"), this);
        connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
        settingsAction = new QAction(tr("&Settings"), this);
        connect(settingsAction, SIGNAL(triggered()), this, SLOT(openSettings()));
        trayMenu = new QMenu();
        trayMenu->addAction(settingsAction);
        trayMenu->addAction(quitAction);

        trayIcon->setContextMenu(trayMenu);
        trayIcon->setIcon(QIcon("://icon.png"));
        trayIcon->show();
    }

    connect(trayIcon, &QSystemTrayIcon::activated, this, &TrayHandler::trayActivate);
}

void TrayHandler::openSettings() {
    //TODO: Open settings window
}

void TrayHandler::trayActivate(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
        case QSystemTrayIcon::Trigger: {
            trayMenu->isVisible()?trayMenu->hide():trayMenu->show();
            break;
        }
        case QSystemTrayIcon::DoubleClick:{
            break;
        }
        case QSystemTrayIcon::MiddleClick: {
            break;
        }
        default: {
        }
    }
}
