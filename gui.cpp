#include "gui.h"
#include "skypush.h"
#include "systemtray.h"
#include "settingsmanager.h"
#include <QHotkey>
#include <QApplication>
#include <QSettings>
#include <QDebug>

GUI::GUI(Skypush *parent)
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
    QString keys = SettingsManager::getOrCreateWithDefault("Shortcuts", "AreaShortcut", "ctrl+shift+4").toString();
    areaHotkey->setShortcut(QKeySequence(keys), true);

    QObject::connect(areaHotkey, &QHotkey::activated, qApp, [&](){
        skypush->grabArea();
    });

    return areaHotkey->isRegistered();
}

bool GUI::registerWindowHotkey()
{
    QString keys = SettingsManager::getOrCreateWithDefault("Shortcuts", "WindowShortcut", "ctrl+shift+3").toString();
    windowHotkey->setShortcut(QKeySequence(keys), true);

    QObject::connect(windowHotkey, &QHotkey::activated, qApp, [&](){
        skypush->grabWindow();
    });

    return windowHotkey->isRegistered();
}

bool GUI::registerEverythingHotkey()
{
    QString keys = SettingsManager::getOrCreateWithDefault("Shortcuts", "EverythingShortcut", "ctrl+shift+2").toString();
    everythingHotkey->setShortcut(QKeySequence(keys), true);

    QObject::connect(everythingHotkey, &QHotkey::activated, qApp, [&](){
        skypush->grabEverything();
    });

    return everythingHotkey->isRegistered();
}
