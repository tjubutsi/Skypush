#include "gui.h"
#include "skypush.h"
#include "systemtray.h"
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
    bool registered = true;
    settings = new QSettings();
    settings->beginGroup("Shortcuts");
    registered = registerAreaHotkey()&registerWindowHotkey()&registerEverythingHotkey();
    settings->endGroup();

    return registered;
}

bool GUI::registerAreaHotkey()
{
    QString keys = "ctrl+alt+4";
    if (settings->contains("AreaShortcut"))
    {
        keys = settings->value("AreaShortcut").toString();
    }
    areaHotkey->setShortcut(QKeySequence(keys), true);

    QObject::connect(areaHotkey, &QHotkey::activated, qApp, [&](){
        skypush->grabArea();
    });

    return areaHotkey->isRegistered();
}

bool GUI::registerWindowHotkey()
{
    QString keys = "ctrl+alt+3";
    if (settings->contains("WindowShortcut"))
    {
        keys = settings->value("WindowShortcut").toString();
    }
    windowHotkey->setShortcut(QKeySequence(keys), true);

    QObject::connect(windowHotkey, &QHotkey::activated, qApp, [&](){
        skypush->grabWindow();
    });

    return windowHotkey->isRegistered();
}

bool GUI::registerEverythingHotkey()
{
    QString keys = "ctrl+alt+2";
    if (settings->contains("EverythingShortcut"))
    {
        keys = settings->value("EverythingShortcut").toString();
    }
    everythingHotkey->setShortcut(QKeySequence(keys), true);

    QObject::connect(everythingHotkey, &QHotkey::activated, qApp, [&](){
        skypush->grabEverything();
    });

    return everythingHotkey->isRegistered();
}
