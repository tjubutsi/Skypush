#include "skypush.h"
#include "gui.h"
#include "systemtray.h"
#include <QApplication>
#include <QHotkey>
#include <QSettings>

GUI::GUI(Skypush *parent) :
    settingsManager(new QSettings())
{
    skypush = parent;
    areaHotkey = new QHotkey(this);
    windowHotkey = new QHotkey(this);
    everythingHotkey = new QHotkey(this);
    systemTray = new SystemTray(this);
}

bool GUI::registerHotkeys() {
    return registerAreaHotkey()&registerWindowHotkey()&registerEverythingHotkey();
}

bool GUI::registerAreaHotkey()
{
    QString keys = settingsManager->value("shortcuts/areaShortcut", "ctrl+shift+4").toString();
    areaHotkey->setShortcut(QKeySequence(keys), true);

    QObject::connect(areaHotkey, &QHotkey::activated, qApp, [&](){
        skypush->grabArea();
    });

    return areaHotkey->isRegistered();
}

bool GUI::registerWindowHotkey()
{
    QString keys = settingsManager->value("shortcuts/windowShortcut", "ctrl+shift+3").toString();
    windowHotkey->setShortcut(QKeySequence(keys), true);

    QObject::connect(windowHotkey, &QHotkey::activated, qApp, [&](){
        skypush->grabWindow();
    });

    return windowHotkey->isRegistered();
}

bool GUI::registerEverythingHotkey()
{
    QString keys = settingsManager->value("shortcuts/everythingShortcut", "ctrl+shift+2").toString();
    everythingHotkey->setShortcut(QKeySequence(keys), true);

    QObject::connect(everythingHotkey, &QHotkey::activated, qApp, [&](){
        skypush->grabEverything();
    });

    return everythingHotkey->isRegistered();
}
