#include "systemtray.h"
#include <QApplication>
#include <QSystemTrayIcon>
#include <QDebug>
#include <QMenu>
#include <QObject>

SystemTray::SystemTray(QObject *parent) : QObject(parent)
{
    createTrayIcon();
    createContextMenu();
}

SystemTray::~SystemTray()
{
    trayIcon->hide();
}

void SystemTray::createTrayIcon()
{
    trayIcon = new QSystemTrayIcon();
    if (trayIcon->isSystemTrayAvailable()) //TODO: settings verification too
    {
        trayIcon->setIcon(QIcon("://images/icon.png"));
        trayIcon->show();
    }

    connect(trayIcon, &QSystemTrayIcon::activated, this, &SystemTray::trayActivate);
}

void SystemTray::createContextMenu()
{
    trayMenu = new QMenu();

    settingsAction = new QAction(tr("&Settings"), this);
    connect(settingsAction, SIGNAL(triggered()), this, SLOT(openSettings()));

    quitAction = new QAction(tr("&Exit"), this);
    connect(quitAction, SIGNAL(triggered()), this, SLOT(exit()));

    trayMenu->addAction(settingsAction);
    trayMenu->addAction(quitAction);

    trayIcon->setContextMenu(trayMenu);
}

void SystemTray::trayActivate(QSystemTrayIcon::ActivationReason reason)
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

void SystemTray::openSettings()
{
    qDebug() << "Opened settings window";
}

void SystemTray::exit()
{
    qApp->quit();
}
