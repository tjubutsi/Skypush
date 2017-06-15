#ifndef SKYPUSH_H
#define SKYPUSH_H

#include <QObject>
#include <QHotkey>

class Skypush : public QObject
{
    Q_OBJECT
    public:
        explicit Skypush(QObject *parent = nullptr);

        QHotkey *areaHotkey;
        QHotkey *windowHotkey;
        QHotkey *everythingHotkey;

    private:

    signals:

    public slots:
        bool registerHotkeys();
        bool registerAreaHotkey();
        bool registerWindowHotkey();
        bool registerEverythingHotkey();
};

#endif // SKYPUSH_H
