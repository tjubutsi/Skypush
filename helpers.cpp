#include "helpers.h"
#include <QHotkey>
#include <QApplication>

Helpers::Helpers(QObject *parent) :
    QObject(parent),
    areaHotkey(new QHotkey(this)),
    windowHotkey(new QHotkey(this)),
    everythingHotkey(new QHotkey(this))
{
}

bool Helpers::registerHotkeys() {
     return registerAreaHotkey()&registerWindowHotkey()&registerEverythingHotkey();
}

bool Helpers::registerAreaHotkey()
{
    areaHotkey->setShortcut(QKeySequence("ctrl+alt+Q"), true);

    QObject::connect(areaHotkey, &QHotkey::activated, qApp, [&](){
    });

    return areaHotkey->isRegistered();
}

bool Helpers::registerWindowHotkey()
{
    windowHotkey->setShortcut(QKeySequence("ctrl+alt+3"), true);

    QObject::connect(windowHotkey, &QHotkey::activated, qApp, [&](){
    });

    return windowHotkey->isRegistered();
}

bool Helpers::registerEverythingHotkey()
{
    everythingHotkey->setShortcut(QKeySequence("ctrl+alt+2"), true);

    QObject::connect(everythingHotkey, &QHotkey::activated, qApp, [&](){
    });

    return everythingHotkey->isRegistered();
}
