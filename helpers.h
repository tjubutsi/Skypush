#ifndef HELPERS_H
#define HELPERS_H

#include <QObject>
#include <QHotkey>

class Helpers : public QObject
{
    Q_OBJECT
    public:
        explicit Helpers(QObject *parent = nullptr);

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

#endif // HELPERS_H
