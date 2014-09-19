#include "settingshelper.h"

#include <QCoreApplication>
#include <QDebug>

SettingsHelper::SettingsHelper()
{
    QCoreApplication::setOrganizationName("Cachirulop");
    QCoreApplication::setOrganizationDomain("cachirulop.com");
    QCoreApplication::setApplicationName("Photo viewer");

    _settings = new QSettings ();
}

SettingsHelper::~SettingsHelper()
{
    delete _settings;
}

QString SettingsHelper::lastDirectory()
{
    return _settings->value("last_directory", "~").toString();
}

void SettingsHelper::setLastDirectory(QString value)
{
    _settings->setValue("last_directory", value);
}

int SettingsHelper::lastFileIndex()
{
    return _settings->value("last_file_index", 0).toInt();
}

void SettingsHelper::setLastFileIndex(int value)
{
    _settings->setValue("last_file_index", value);
}
