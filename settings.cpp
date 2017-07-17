#include "settings.h"
#include "ui_settings.h"
#include "systemtray.h"
#include "gui.h"
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
    systemTray->gui->settings->beginGroup("Shortcuts");
    systemTray->gui->settings->setValue("AreaShortcut", systemTray->gui->areaHotkey->shortcut());
    systemTray->gui->settings->setValue("WindowShortcut", systemTray->gui->windowHotkey->shortcut());
    systemTray->gui->settings->setValue("EverythingShortcut", systemTray->gui->everythingHotkey->shortcut());
    systemTray->gui->settings->endGroup();
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
