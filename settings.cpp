#include "skypush.h"
#include "gui.h"
#include "systemtray.h"
#include "settings.h"
#include "ui_settings.h"
#include <QHotkey>

Settings::Settings(SystemTray *parent) :
    ui(new Ui::Settings),
    settingsManager(new QSettings())
{
    skypush = parent->gui->skypush;
    gui = parent->gui;
    systemTray = parent;
    ui->setupUi(this);
    ui->editAreaHotkey->setKeySequence(gui->areaHotkey->shortcut());
    ui->editWindowHotkey->setKeySequence(gui->windowHotkey->shortcut());
    ui->editEverythingHotkey->setKeySequence(gui->everythingHotkey->shortcut());
    ui->tokenLineEdit->setText(gui->skypush->token);
    connect(ui->editAreaHotkey, &QKeySequenceEdit::editingFinished, this, &Settings::checkSettings);
    connect(ui->editWindowHotkey, &QKeySequenceEdit::editingFinished, this, &Settings::checkSettings);
    connect(ui->editEverythingHotkey, &QKeySequenceEdit::editingFinished, this, &Settings::checkSettings);
}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_editAreaHotkey_keySequenceChanged(const QKeySequence &keySequence)
{
    if (keySequence.count() > 0)
    {
        ui->editAreaHotkey->setKeySequence(keySequence[0]);
    }
}

void Settings::on_editWindowHotkey_keySequenceChanged(const QKeySequence &keySequence)
{
    if (keySequence.count() > 0)
    {
        ui->editWindowHotkey->setKeySequence(keySequence[0]);
    }
}

void Settings::on_editEverythingHotkey_keySequenceChanged(const QKeySequence &keySequence)
{
    if (keySequence.count() > 0)
    {
        ui->editEverythingHotkey->setKeySequence(keySequence[0]);
    }
}

void Settings::checkSettings()
{
    if (ui->editAreaHotkey->keySequence().count() == 0 || ui->editWindowHotkey->keySequence().count() == 0 || ui->editEverythingHotkey->keySequence().count() == 0)
    {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
    else
    {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
}

void Settings::on_regenerateTokenButton_clicked()
{
    getNewToken();
}

void Settings::getNewToken()
{
    QNetworkRequest request(QUrl("https://skyweb.nu/api/init.php"));
    request.setRawHeader("token", skypush->token.toUtf8());
    QNetworkReply* reply = skypush->networkManager->get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(getTokenReplyFinished()));
}

void Settings::getTokenReplyFinished()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    QJsonObject result = Skypush::jsonToObject(reply->readAll());
    QString message = result["message"].toString();

    if (reply->error() == QNetworkReply::NoError)
    {
        ui->tokenLineEdit->setText(message);
    }
    else
    {
        systemTray->trayIcon->showMessage("Failed", reply->errorString(), QSystemTrayIcon::Critical, 5000);
    }

    reply->deleteLater();
}

void Settings::setNewToken()
{
    QNetworkRequest request(QUrl("https://skyweb.nu/api/convertToken.php"));
    request.setRawHeader("token", skypush->token.toUtf8());
    QByteArray data = "newToken=";
    data.append(ui->tokenLineEdit->text());
    QNetworkReply* reply = skypush->networkManager->post(request, data);
    connect(reply, SIGNAL(finished()), this, SLOT(setTokenReplyFinished()));
}

void Settings::setTokenReplyFinished()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    QJsonObject result = Skypush::jsonToObject(reply->readAll());

    if (reply->error() == QNetworkReply::NoError)
    {
        settingsManager->setValue("program/token", ui->tokenLineEdit->text());
        skypush->token = settingsManager->value("program/token").toString();
        accept();
    }
    else
    {
        systemTray->trayIcon->showMessage("Failed", reply->errorString(), QSystemTrayIcon::Critical, 5000);
    }

    reply->deleteLater();
}

void Settings::on_buttonBox_accepted()
{
    checkSettings();
    if (!ui->buttonBox->button(QDialogButtonBox::Ok)->isEnabled())
    {
        return;
    }
    setNewToken();
    QKeySequence areaSequence = ui->editAreaHotkey->keySequence()[0];
    QKeySequence windowSequence = ui->editAreaHotkey->keySequence()[0];
    QKeySequence everythingSequence = ui->editEverythingHotkey->keySequence()[0];
    settingsManager->setValue("shortcuts/areaShortcut", areaSequence.toString());
    settingsManager->setValue("shortcuts/windowShortcut", windowSequence.toString());
    settingsManager->setValue("shortcuts/everythingShortcut", everythingSequence.toString());

    gui->areaHotkey->setShortcut(settingsManager->value("shortcuts/areaShortcut").toString(), false);
    gui->areaHotkey->setRegistered(true);
    gui->windowHotkey->setShortcut(settingsManager->value("shortcuts/windowShortcut").toString(), false);
    gui->windowHotkey->setRegistered(true);
    gui->everythingHotkey->setShortcut(settingsManager->value("shortcuts/everythingShortcut").toString(), false);
    gui->everythingHotkey->setRegistered(true);
}
