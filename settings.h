#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QNetworkReply>
#include <QSettings>

class Skypush;
class GUI;
class SystemTray;

namespace Ui {
    class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

    public:
        explicit Settings(SystemTray *parent = nullptr);
        ~Settings();

        QSettings *settingsManager;

        //external
        Skypush *skypush;
        GUI *gui;
        SystemTray *systemTray;

    private slots:
        void on_editAreaHotkey_keySequenceChanged(const QKeySequence &keySequence);
        void on_editWindowHotkey_keySequenceChanged(const QKeySequence &keySequence);
        void on_editEverythingHotkey_keySequenceChanged(const QKeySequence &keySequence);
        void checkSettings();
        void on_regenerateTokenButton_clicked();
        void getNewToken();
        void getTokenReplyFinished();
        void setNewToken();
        void setTokenReplyFinished();
        void on_buttonBox_accepted();

    private:
        Ui::Settings *ui;
};

#endif // SETTINGS_H
