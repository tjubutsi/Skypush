#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QNetworkReply>
#include <QSettings>

class GUI;
class Skypush;

namespace Ui {
    class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

    public:
        explicit Settings(GUI *parent = nullptr);
        ~Settings();

        QSettings *settingsManager;

        //external
        Skypush *skypush;
        GUI *gui;

    private slots:
        void on_editAreaHotkey_keySequenceChanged(const QKeySequence &keySequence);
        void on_editWindowHotkey_keySequenceChanged(const QKeySequence &keySequence);
        void on_editEverythingHotkey_keySequenceChanged(const QKeySequence &keySequence);
        void checkSettings();
        void on_buttonBox_accepted();
        void on_buttonBox_rejected();

private:
        Ui::Settings *ui;
};

#endif // SETTINGS_H
