#include "settings.h"
#include "ui_settings.h"
#include "systemtray.h"
#include "gui.h"
#include "settingsmanager.h"
#include "skypush.h"
#include <QHotkey>
#include <QDebug>
#include <QPushButton>
#include <QWidget>

Settings::Settings(SystemTray *parent) :
    ui(new Ui::Settings)
{
    systemTray = parent;
    ui->setupUi(this);
    ui->editAreaHotkey->setKeySequence(systemTray->gui->areaHotkey->shortcut());
    ui->editWindowHotkey->setKeySequence(systemTray->gui->windowHotkey->shortcut());
    ui->editEverythingHotkey->setKeySequence(systemTray->gui->everythingHotkey->shortcut());
    ui->tokenLineEdit->setText(SettingsManager::getValue("program", "token").toString());
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

void Settings::on_buttonBox_accepted()
{
    checkSettings();
    if (!ui->buttonBox->button(QDialogButtonBox::Ok)->isEnabled())
    {
        return;
    }
    systemTray->gui->areaHotkey->setShortcut(ui->editAreaHotkey->keySequence()[0], false);
    systemTray->gui->windowHotkey->setShortcut(ui->editWindowHotkey->keySequence()[0], false);
    systemTray->gui->everythingHotkey->setShortcut(ui->editEverythingHotkey->keySequence()[0], false);
    systemTray->gui->areaHotkey->setRegistered(true);
    systemTray->gui->windowHotkey->setRegistered(true);
    systemTray->gui->everythingHotkey->setRegistered(true);
    systemTray->gui->skypush->token = ui->tokenLineEdit->text();
    SettingsManager::setValue("program", "token", ui->tokenLineEdit->text());
    SettingsManager::setValue("Shortcuts", "AreaShortcut", systemTray->gui->areaHotkey->shortcut());
    SettingsManager::setValue("Shortcuts", "WindowShortcut", systemTray->gui->windowHotkey->shortcut());
    SettingsManager::setValue("Shortcuts", "EverythingShortcut", systemTray->gui->everythingHotkey->shortcut());
    accept();
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
    QNetworkReply* reply = systemTray->gui->skypush->manager->get(request);
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
