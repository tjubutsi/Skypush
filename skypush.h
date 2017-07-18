#ifndef SKYPUSH_H
#define SKYPUSH_H

#include <QObject>
#include <QPixmap>
#include <QNetworkReply>
#include <QRubberBand>
#include <QJsonObject>
#include <QSettings>

class AreaWindow;
class GUI;

class Skypush : public QObject
{
    Q_OBJECT
    public:
        explicit Skypush(QObject *parent = nullptr);
        GUI *gui;
        QNetworkAccessManager *manager;
        AreaWindow *areaWindow;
        QSettings *settings;
        QString token;

    signals:

    public slots:
        QPixmap getAllMonitorsPixmap();
        QPixmap getAreaPixmap(QRect rect);
        void grabAreaAndUpload(QRect rect);
        QByteArray convertToByteArray(QPixmap ScreenGrab);
        void upload(QByteArray ByteArray);
        void replyFinished();
        void tokenReplyFinished();
        void grabArea();
        void grabWindow();
        void grabEverything();
        static QJsonObject jsonToObject(QByteArray bytes);
        void getNewToken();
};

#endif // SKYPUSH_H
