#include "skypush.h"
#include "systemtray.h"
#include "gui.h"
#include "areawindow.h"
#include <QHotkey>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QPixmap>
#include <QScreen>
#include <QBuffer>
#include <QtNetwork>
#include <QClipboard>
#if defined(Q_OS_WIN)
    #include "windows.h"
    #include "dwmapi.h"
#endif

Skypush::Skypush(QObject *parent) :
    QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    gui = new GUI(this);
    if (!gui->registerHotkeys())
    {
        qDebug() << "registering failed";
        qApp->quit();
    }
}

QPixmap Skypush::getAllMonitorsPixmap()
{
    QList<QScreen *> screens = QGuiApplication::screens();
    QScreen *topleft = QApplication::primaryScreen();
    QRect rect;
    foreach (auto screen, screens) {
        rect = rect.united(screen->geometry());
        if (topleft->geometry().x() > screen->geometry().x() || topleft->geometry().y() > screen->geometry().y())
        {
            topleft = screen;
        }
    }
    qDebug() << rect;
    return topleft->grabWindow(0, rect.x(), rect.y(), rect.width(), rect.height());
}

QPixmap Skypush::getAreaPixmap(QRect rect)
{
    QPixmap full = getAllMonitorsPixmap();
    return full.copy(rect);
}

void Skypush::grabAreaAndUpload(QRect rect)
{
    QPixmap screengrab = getAreaPixmap(rect);
    QByteArray byteArray = convertToByteArray(screengrab);
    upload(byteArray);
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

    QNetworkReply* reply = manager->post(QNetworkRequest(QUrl("https://skyweb.nu/api/upload.php")), multiPart);
    connect(reply, SIGNAL(finished()), this, SLOT(replyFinished()));
}

void Skypush::replyFinished()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    QByteArray content= reply->readAll();

    if (reply->error() == QNetworkReply::NoError)
    {
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(content);
        gui->systemTray->trayIcon->showMessage("Success", content, QSystemTrayIcon::NoIcon, 5000);
    }
    else
    {
        gui->systemTray->trayIcon->showMessage("Failed", content, QSystemTrayIcon::Critical, 5000);
    }

    gui->systemTray->trayIcon->setToolTip("Skypush");
    reply->deleteLater();
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
        //if (IsZoomed(window))
        //{
        //    r.bottom -= 8;
        //    r.top += 8;
        //    r.left += 8;
        //    r.right -= 8;
        //}
        //else
        //{
        //    r.bottom -= 7;
        //    r.left += 7;
        //    r.right -= 7;
        //}
        QRect rect = QRect(r.left, r.top, r.right - r.left, r.bottom - r.top);
        grabAreaAndUpload(rect);
    #elif defined(Q_OS_LINUX)
        //Window focus;
        //int revert;
        //
        //XGetInputFocus(QX11Info::display(), &focus, &revert);
        //XGetWindowAttributes(QX11Info::display(), focus, &attr);



        //QRect rect = QRect(attributes.x, attributes.y, attributes.width, attributes.height);
        //grabAreaAndUpload(rect);
        //QScreen *screen = QGuiApplication::primaryScreen();
        //QPixmap screengrab = screen->grabWindow(window);
        //QByteArray byteArray = convertToByteArray(screengrab);
        //upload(byteArray);
    #endif
}

void Skypush::grabEverything()
{
    QPixmap screengrab = getAllMonitorsPixmap();
    QByteArray byteArray = convertToByteArray(screengrab);
    upload(byteArray);
}
