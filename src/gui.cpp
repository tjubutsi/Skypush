#include "skypush.h"
#include "gui.h"
#include "systemtray.h"
#include <QApplication>
#include <QHotkey>
#include <QNetworkReply>
#include <QSettings>

GUI::GUI(Skypush *parent) :
    settingsManager(new QSettings())
{
    skypush = parent;
    systemTray = new SystemTray(this);
    areaHotkey = new QHotkey(this);
    windowHotkey = new QHotkey(this);
    everythingHotkey = new QHotkey(this);
    privateUpload = settingsManager->value("privateUpload", false).toBool();
    setClientToken();
    setAccessToken();
}

bool GUI::registerHotkeys() {
    return registerAreaHotkey()&registerWindowHotkey()&registerEverythingHotkey();
}

bool GUI::registerAreaHotkey()
{
    QString keys = settingsManager->value("shortcuts/areaShortcut", "ctrl+shift+4").toString();
    areaHotkey->setShortcut(QKeySequence(keys), true);

    QObject::connect(areaHotkey, &QHotkey::activated, qApp, [&](){
        skypush->grabArea();
    });

    return areaHotkey->isRegistered();
}

bool GUI::registerWindowHotkey()
{
    QString keys = settingsManager->value("shortcuts/windowShortcut", "ctrl+shift+3").toString();
    windowHotkey->setShortcut(QKeySequence(keys), true);

    QObject::connect(windowHotkey, &QHotkey::activated, qApp, [&](){
        skypush->grabWindow();
    });

    return windowHotkey->isRegistered();
}

bool GUI::registerEverythingHotkey()
{
    QString keys = settingsManager->value("shortcuts/everythingShortcut", "ctrl+shift+2").toString();
    everythingHotkey->setShortcut(QKeySequence(keys), true);

    QObject::connect(everythingHotkey, &QHotkey::activated, qApp, [&](){
        skypush->grabEverything();
    });

    return everythingHotkey->isRegistered();
}

void GUI::setClientToken()
{
    if (settingsManager->contains("clientToken"))
    {
        clientToken = settingsManager->value("clientToken").toString();
    }
    else
    {
        getClientToken();
    }
}

void GUI::getClientToken()
{
    QNetworkRequest request(QUrl("https://skyweb.nu/api/registerClient.php"));
    QNetworkReply* reply = skypush->networkManager->get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(getClientTokenReply()));
}

void GUI::getClientTokenReply()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    QJsonObject response = Skypush::jsonToObject(reply->readAll());
    if (reply->error() == QNetworkReply::NoError)
    {
        settingsManager->setValue("clientToken", response["message"].toString());
        clientToken = response["message"].toString();
    }
    else
    {
        qDebug() << "shit's fucked";
    }

    reply->deleteLater();
}

void GUI::setAccessToken()
{
    if (settingsManager->contains("sessionToken"))
    {
        sessionToken = settingsManager->value("sessionToken").toString();
        getAccessToken();
    }
}

void GUI::getAccessToken()
{
    QNetworkRequest request(QUrl("https://skyweb.nu/api/auth.php"));
    request.setRawHeader("token", sessionToken.toUtf8());
    QByteArray empty;
    QNetworkReply* reply = skypush->networkManager->post(request, empty);
    connect(reply, SIGNAL(finished()), this, SLOT(getAccessTokenReply()));
}

void GUI::getAccessTokenReply()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    QJsonObject response = Skypush::jsonToObject(reply->readAll());
    if (reply->error() == QNetworkReply::NoError)
    {
        accessToken = response["message"].toString();
    }
    else
    {
        qDebug() << "shit's fucked";
        systemTray->trayIcon->showMessage("Failed", response["message"].toString(), QSystemTrayIcon::Information, 5000);
    }

    reply->deleteLater();
}
