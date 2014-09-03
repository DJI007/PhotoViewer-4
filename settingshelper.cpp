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
