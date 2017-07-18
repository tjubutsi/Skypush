#include "settingsmanager.h"
#include <QSettings>
#include <QString>
#include <QVariant>

void SettingsManager::setValue(QString group, QString property, QVariant value)
{
    QSettings settings;
    settings.beginGroup(group);
    settings.setValue(property, value);
    settings.endGroup();
}

QVariant SettingsManager::getValue(QString group, QString property)
{
    QVariant result;
    QSettings settings;
    settings.beginGroup(group);
    result = settings.value(property);
    settings.endGroup();
    return result;
}

bool SettingsManager::propertyExists(QString group, QString property)
{
    bool result;
    QSettings settings;
    settings.beginGroup(group);
    result = settings.contains(property);
    settings.endGroup();
    return result;
}

QVariant SettingsManager::getOrCreateWithDefault(QString group, QString property, QVariant defaultValue)
{
    if (!SettingsManager::propertyExists(group, property))
    {
        SettingsManager::setValue(group, property, defaultValue);
    }
    return SettingsManager::getValue(group, property);
}
