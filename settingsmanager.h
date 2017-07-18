#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QVariant>
#include <QString>

class SettingsManager : public QObject
{
    Q_OBJECT

    public slots:
        static void setValue(QString group, QString property, QVariant value);
        static QVariant getValue(QString group, QString property);
        static bool propertyExists(QString group, QString property);
        static QVariant getOrCreateWithDefault(QString group, QString property, QVariant defaultValue);
};

#endif // SETTINGSMANAGER_H
