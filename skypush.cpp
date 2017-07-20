#include "skypush.h"
#include "gui.h"
#include "systemtray.h"
#include "areawindow.h"
#include <QApplication>
#include <QBuffer>
#include <QClipboard>
#include <QDesktopWidget>
#include <QHttpMultiPart>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QScreen>
#if defined(Q_OS_WIN)
    #include "windows.h"
    #include "dwmapi.h"
#endif

Skypush::Skypush(QObject *parent) :
    QObject(parent),
    settingsManager(new QSettings())
{
    networkManager = new QNetworkAccessManager(this);
    gui = new GUI(this);
    if (!gui->registerHotkeys())
    {
        qDebug() << "registering failed2"; //show message and settings
        qApp->quit();
    }
    if (settingsManager->contains("program/token"))
    {
        token = settingsManager->value("program/token").toString();
    }
    else
    {
        getNewToken();
    }
}

QJsonObject Skypush::jsonToObject(QByteArray bytes)
{
    QJsonDocument jsonDocument(QJsonDocument::fromJson(bytes));
    return jsonDocument.object();
}

void Skypush::grabArea()
{
    QScreen *screen = QApplication::primaryScreen();
    QRect rect = screen->virtualGeometry();
    areaWindow = new AreaWindow(rect, this);
    areaWindow->show();
}

void Skypush::grabWindow()
{
    #if defined(Q_OS_WIN)
        HWND window = GetForegroundWindow();
        RECT r;
        ::DwmGetWindowAttribute(window, DWMWA_EXTENDED_FRAME_BOUNDS, &r, sizeof(r));
        QRect rect = QRect(r.left, r.top, r.right - r.left, r.bottom - r.top);
        grabAreaAndUpload(rect);
    #elif defined(Q_OS_LINUX)
    #endif
}

void Skypush::grabAreaAndUpload(QRect rect)
{
    QPixmap screengrab = getAreaPixmap(rect);
    QByteArray byteArray = convertToByteArray(screengrab);
    upload(byteArray);
}

void Skypush::grabEverything()
{
    QPixmap screengrab = getAllMonitorsPixmap();
    QByteArray byteArray = convertToByteArray(screengrab);
    upload(byteArray);
}

QPixmap Skypush::getAllMonitorsPixmap()
{
    QScreen *screen = QApplication::primaryScreen();
    QDesktopWidget* dw = QApplication::desktop();
    return screen->grabWindow(dw->winId(), 0, 0, dw->width(), dw->height());
}

QPixmap Skypush::getAreaPixmap(QRect rect)
{
    QPixmap full = getAllMonitorsPixmap();
    return full.copy(rect);
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
    gui->systemTray->trayIcon->setToolTip("Skypush - Uploading...");

    QHttpPart imagePart;
    imagePart.setBody(ByteArray);
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"data\"; filename=\"tmp_name\""));

    QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    multiPart->append(imagePart);

    QNetworkRequest request(QUrl("https://skyweb.nu/api/upload.php"));
    request.setRawHeader("token", token.toUtf8());
    QNetworkReply* reply = networkManager->post(request, multiPart);
    connect(reply, SIGNAL(finished()), this, SLOT(replyFinished()));
}

void Skypush::replyFinished()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    QJsonObject result = jsonToObject(reply->readAll());
    QString message = result["message"].toString();

    if (reply->error() == QNetworkReply::NoError)
    {
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(message);
        gui->systemTray->trayIcon->showMessage("Success", message, QSystemTrayIcon::Information, 5000);
    }
    else
    {
        gui->systemTray->trayIcon->showMessage("Failed", message, QSystemTrayIcon::Critical, 5000);
    }

    gui->systemTray->trayIcon->setToolTip("Skypush");
    reply->deleteLater();
}

void Skypush::getNewToken()
{
    QNetworkRequest request(QUrl("https://skyweb.nu/api/init.php"));
    QNetworkReply* reply = networkManager->get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(tokenReplyFinished()));
}

void Skypush::tokenReplyFinished()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    QJsonObject result = jsonToObject(reply->readAll());
    QString message = result["message"].toString();

    if (reply->error() == QNetworkReply::NoError)
    {
        settingsManager->setValue("program/token", message);
        token = settingsManager->value("program/token").toString();
    }
    else
    {
        gui->systemTray->trayIcon->showMessage("Failed", reply->errorString(), QSystemTrayIcon::Critical, 5000);
    }

    reply->deleteLater();
}
