#ifndef GUI_H
#define GUI_H

#include <QObject>
#include <QHotkey>

class Skypush;
class SystemTray;

class GUI : public QObject
{
    Q_OBJECT

    public:
        explicit GUI(Skypush *parent = nullptr);
        Skypush *skypush;
        QHotkey *areaHotkey;
        QHotkey *windowHotkey;
        QHotkey *everythingHotkey;
        SystemTray *systemTray;

    public slots:
        bool registerHotkeys();
        bool registerAreaHotkey();
        bool registerWindowHotkey();
        bool registerEverythingHotkey();
};

#endif // GUI_H
