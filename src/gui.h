#ifndef GUI_H
#define GUI_H

#include <QHotkey>
#include <QObject>
#include <QSettings>

class Skypush;
class SystemTray;

class GUI : public QObject
{
    Q_OBJECT

    public:
        explicit GUI(Skypush *parent = nullptr);

        QSettings *settingsManager;

        //external
        Skypush *skypush;

        SystemTray *systemTray;
        QHotkey *areaHotkey;
        QHotkey *windowHotkey;
        QHotkey *everythingHotkey;

    public slots:
        bool registerHotkeys();
        bool registerAreaHotkey();
        bool registerWindowHotkey();
        bool registerEverythingHotkey();
};

#endif // GUI_H
