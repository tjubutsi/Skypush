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
    systemTray = parent;
    gui = parent->gui;
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
    QNetworkRequest request(QUrl("https://skyweb.nu/api2/init.php"));
    QNetworkReply* reply = systemTray->gui->skypush->networkManager->get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(tokenReplyFinished()));
}

void Settings::tokenReplyFinished()
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
        gui->systemTray->trayIcon->showMessage("Failed", reply->errorString(), QSystemTrayIcon::Critical, 5000);
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
    settingsManager->setValue("shortcuts/areaShortcut", ui->editAreaHotkey->keySequence()[0]);
    settingsManager->setValue("shortcuts/windowShortcut", ui->editAreaHotkey->keySequence()[0]);
    settingsManager->setValue("shortcuts/everythingShortcut", ui->editEverythingHotkey->keySequence()[0]);
    settingsManager->setValue("program/token", ui->tokenLineEdit->text());

    gui->areaHotkey->setShortcut(settingsManager->value("shortcuts/areaShortcut").toString(), false);
    gui->areaHotkey->setRegistered(true);
    gui->windowHotkey->setShortcut(settingsManager->value("shortcuts/windowShortcut").toString(), false);
    gui->windowHotkey->setRegistered(true);
    gui->everythingHotkey->setShortcut(settingsManager->value("shortcuts/everythingShortcut").toString(), false);
    gui->everythingHotkey->setRegistered(true);

    gui->skypush->token = settingsManager->value("program/token").toString();
    accept();
}
