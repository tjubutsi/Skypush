#include "skypush.h"
#include "systemtray.h"
#include <QHotkey>
#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QPixmap>
#include <QScreen>
#include <QBuffer>
#include <QtNetwork>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QHttpMultiPart>
#include <QUrl>
#include <QClipboard>

Skypush::Skypush(QObject *parent) :
    QObject(parent),
    areaHotkey(new QHotkey(this)),
    windowHotkey(new QHotkey(this)),
    everythingHotkey(new QHotkey(this))
{
}

bool Skypush::registerHotkeys() {
     return registerAreaHotkey()&registerWindowHotkey()&registerEverythingHotkey();
}

bool Skypush::registerAreaHotkey()
{
    areaHotkey->setShortcut(QKeySequence("ctrl+alt+4"), true);

    QObject::connect(areaHotkey, &QHotkey::activated, qApp, [&](){
        qDebug() << "Area hotkey pressed";
    });

    return areaHotkey->isRegistered();
}

bool Skypush::registerWindowHotkey()
{
    windowHotkey->setShortcut(QKeySequence("ctrl+alt+3"), true);

    QObject::connect(windowHotkey, &QHotkey::activated, qApp, [&](){
        qDebug() << "Window hotkey pressed";
    });

    return windowHotkey->isRegistered();
}

bool Skypush::registerEverythingHotkey()
{
    everythingHotkey->setShortcut(QKeySequence("ctrl+alt+2"), true);

    QObject::connect(everythingHotkey, &QHotkey::activated, this, &Skypush::grabEverything);

    return everythingHotkey->isRegistered();
}

void Skypush::grabEverything()
{
    QPixmap screengrab = getAllMonitorsPixmap();
    QByteArray byteArray = convertToByteArray(screengrab);
    upload(byteArray);
}

QPixmap Skypush::getAllMonitorsPixmap()
{
    QDesktopWidget *desktop = QApplication::desktop();
    QScreen *screen = QApplication::primaryScreen();
    return screen->grabWindow(desktop->winId(), 0, 0, desktop->width(), desktop->height());
}

QByteArray Skypush::convertToByteArray(QPixmap ScreenGrab)
{
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::ReadWrite);
    ScreenGrab.save(&buffer, "PNG");
    buffer.close();
    return byteArray;
}

void Skypush::upload(QByteArray ByteArray)
{
    QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart imagePart;
    imagePart.setBody(ByteArray);
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"data\"; filename=\"tmp_name\""));

    multiPart->append(imagePart);

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    manager->post(QNetworkRequest(QUrl("https://skyweb.nu/api/upload.php")), multiPart);
}

void Skypush::replyFinished(QNetworkReply *reply)
{
    QByteArray content= reply->readAll();
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(content);
}
