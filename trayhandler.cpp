#include "trayhandler.h"
#include "helpers.h"
#include <QApplication>
#include <QSystemTrayIcon>
#include <QDebug>
#include <QMenu>

TrayHandler::TrayHandler(QObject *parent) : QObject(parent)
{
    createTrayIcon();
    createContextMenu();
}

void TrayHandler::createTrayIcon()
{
    trayIcon = new QSystemTrayIcon();
    if (trayIcon->isSystemTrayAvailable()) //TODO: settings verification too
    {
        trayIcon->setIcon(QIcon("://icon.png"));
        trayIcon->show();
    }

    connect(trayIcon, &QSystemTrayIcon::activated, this, &TrayHandler::trayActivate);
}

void TrayHandler::createContextMenu()
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

void TrayHandler::openSettings()
{
    qDebug() << "Opened settings window";
}

void TrayHandler::exit()
{
    trayIcon->hide();
    qApp->quit();
}
