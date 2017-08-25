#include "skypush.h"
#include "gui.h"
#include "systemtray.h"
#include "settings.h"
#include "ui_settings.h"
#include <QHotkey>
#include <QPushButton>

Settings::Settings(GUI *parent) :
    ui(new Ui::Settings),
    settingsManager(new QSettings())
{
    skypush = parent->skypush;
    gui = parent;
    ui->setupUi(this);
    ui->editAreaHotkey->setKeySequence(settingsManager->value("shortcuts/areaShortcut", "ctrl+shift+4").toString());
    ui->editWindowHotkey->setKeySequence(settingsManager->value("shortcuts/windowShortcut", "ctrl+shift+3").toString());
    ui->editEverythingHotkey->setKeySequence(settingsManager->value("shortcuts/everythingShortcut", "ctrl+shift+2").toString());
    ui->privateUploadCheckBox->setChecked(settingsManager->value("privateUpload", false).toBool());
    gui->areaHotkey->setRegistered(false);
    gui->windowHotkey->setRegistered(false);
    gui->everythingHotkey->setRegistered(false);
    connect(ui->editAreaHotkey, &QKeySequenceEdit::editingFinished, this, &Settings::checkSettings);
    connect(ui->editWindowHotkey, &QKeySequenceEdit::editingFinished, this, &Settings::checkSettings);
    connect(ui->editEverythingHotkey, &QKeySequenceEdit::editingFinished, this, &Settings::checkSettings);
    ui->privateUploadCheckBox->setEnabled(settingsManager->contains("sessionToken"));
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

void Settings::on_buttonBox_accepted()
{
    checkSettings();
    if (!ui->buttonBox->button(QDialogButtonBox::Ok)->isEnabled())
    {
        return;
    }
    QKeySequence areaSequence = ui->editAreaHotkey->keySequence()[0];
    QKeySequence windowSequence = ui->editWindowHotkey->keySequence()[0];
    QKeySequence everythingSequence = ui->editEverythingHotkey->keySequence()[0];
    settingsManager->setValue("shortcuts/areaShortcut", areaSequence.toString());
    settingsManager->setValue("shortcuts/windowShortcut", windowSequence.toString());
    settingsManager->setValue("shortcuts/everythingShortcut", everythingSequence.toString());
    settingsManager->setValue("privateUpload", ui->privateUploadCheckBox->isChecked());

    gui->areaHotkey->setShortcut(settingsManager->value("shortcuts/areaShortcut").toString(), false);
    gui->areaHotkey->setRegistered(true);
    gui->windowHotkey->setShortcut(settingsManager->value("shortcuts/windowShortcut").toString(), false);
    gui->windowHotkey->setRegistered(true);
    gui->everythingHotkey->setShortcut(settingsManager->value("shortcuts/everythingShortcut").toString(), false);
    gui->everythingHotkey->setRegistered(true);
    gui->privateUpload = settingsManager->value("privateUpload").toBool();
    settingsManager->sync();
    accept();
}

void Settings::on_buttonBox_rejected()
{
    gui->areaHotkey->setShortcut(settingsManager->value("shortcuts/areaShortcut").toString(), false);
    gui->areaHotkey->setRegistered(true);
    gui->windowHotkey->setShortcut(settingsManager->value("shortcuts/windowShortcut").toString(), false);
    gui->windowHotkey->setRegistered(true);
    gui->everythingHotkey->setShortcut(settingsManager->value("shortcuts/everythingShortcut").toString(), false);
    gui->everythingHotkey->setRegistered(true);
    gui->privateUpload = settingsManager->value("privateUpload").toBool();
    reject();
}
