#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>

class SystemTray;
class GUI;

namespace Ui {
    class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

    public:
        explicit Settings(SystemTray *parent = nullptr);
        ~Settings();
        SystemTray *systemTray;
        GUI *gui;

    private slots:
        void on_editAreaHotkey_keySequenceChanged(const QKeySequence &keySequence);
        void on_editWindowHotkey_keySequenceChanged(const QKeySequence &keySequence);
        void on_editEverythingHotkey_keySequenceChanged(const QKeySequence &keySequence);
        void on_buttonBox_accepted();
        void checkSettings();

private:
        Ui::Settings *ui;
};

#endif // SETTINGS_H
