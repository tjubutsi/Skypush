#include "systemtray.h"
#include "settings.h"
#include "account.h"
#include "gui.h"
#include <QApplication>

SystemTray::SystemTray(GUI *parent)
{
    gui = parent;
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
    if (trayIcon->isSystemTrayAvailable())
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

    accountAction = new QAction(tr("&Account"), this);
    connect(accountAction, SIGNAL(triggered()), this, SLOT(openAccount()));

    quitAction = new QAction(tr("&Exit"), this);
    connect(quitAction, SIGNAL(triggered()), this, SLOT(exit()));

    trayMenu->addAction(settingsAction);
    trayMenu->addAction(accountAction);
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
    gui->settings = new Settings(gui);
    gui->settings->show();
}

void SystemTray::openAccount()
{
    gui->account = new Account(gui);
    gui->account->show();
}

void SystemTray::exit()
{
    trayIcon->hide();
    qApp->quit();
}
