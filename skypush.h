#ifndef SKYPUSH_H
#define SKYPUSH_H

#include <QObject>
#include <QHotkey>
#include <QPixmap>
#include <QNetworkReply>
#include "systemtray.h"

class Skypush : public QObject
{
    Q_OBJECT
    public:
        explicit Skypush(QObject *parent = nullptr);

        QHotkey *areaHotkey;
        QHotkey *windowHotkey;
        QHotkey *everythingHotkey;

        SystemTray *systemTray;

    private:
        QNetworkAccessManager *manager;

    signals:

    public slots:
        bool registerHotkeys();
        bool registerAreaHotkey();
        bool registerWindowHotkey();
        bool registerEverythingHotkey();
        void replyFinished();

    private slots:
        void grabEverything();
        QPixmap getAllMonitorsPixmap();
        QByteArray convertToByteArray(QPixmap ScreenGrab);
        void upload(QByteArray ByteArray);
};

#endif // SKYPUSH_H
