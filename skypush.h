#ifndef SKYPUSH_H
#define SKYPUSH_H

#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QObject>
#include <QPixmap>
#include <QSettings>

class GUI;
class AreaWindow;

class Skypush : public QObject
{
    Q_OBJECT
    public:
        explicit Skypush(QObject *parent = nullptr);

        QSettings *settingsManager;

        //external
        GUI *gui;

        QNetworkAccessManager *networkManager;
        QString token;

    signals:

    public slots:
        static QJsonObject jsonToObject(QByteArray bytes);

        void grabArea();
        void grabWindow();
        void grabEverything();
        void grabAreaAndUpload(QRect rect);
        QPixmap getAllMonitorsPixmap();
        QPixmap getAreaPixmap(QRect rect);
        QByteArray convertToByteArray(QPixmap ScreenGrab);
        void upload(QByteArray ByteArray);
        void replyFinished();
        void getNewToken();
        void tokenReplyFinished();

    private:
        AreaWindow *areaWindow;
};

#endif // SKYPUSH_H
