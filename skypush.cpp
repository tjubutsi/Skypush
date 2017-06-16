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
#include <QClipboard>

Skypush::Skypush(QObject *parent) :
    QObject(parent),
    areaHotkey(new QHotkey(this)),
    windowHotkey(new QHotkey(this)),
    everythingHotkey(new QHotkey(this)),
    systemTray(new SystemTray(this))
{
    manager = new QNetworkAccessManager(this);
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
    systemTray->trayIcon->setToolTip("Skypush - Uploading...");

    QHttpPart imagePart;
    imagePart.setBody(ByteArray);
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"data\"; filename=\"tmp_name\""));

    QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    multiPart->append(imagePart);

    QNetworkReply* reply = manager->post(QNetworkRequest(QUrl("https://skyweb.nu/api/upload.php")), multiPart);
    connect(reply, SIGNAL(finished()), this, SLOT(replyFinished()));
}

void Skypush::replyFinished()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray content= reply->readAll();
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(content);
        systemTray->trayIcon->showMessage("Success", content, QSystemTrayIcon::NoIcon, 5000);
    }
    else
    {
        systemTray->trayIcon->showMessage("Success", reply->errorString(), QSystemTrayIcon::Critical, 5000);
    }

    systemTray->trayIcon->setToolTip("Skypush");
    reply->deleteLater();
}
