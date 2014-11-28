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

int SettingsHelper::volume()
{
    return _settings->value("volume", 50).toInt();
}

void SettingsHelper::setVolume(int value)
{
    _settings->setValue("volume", value);
}

QString SettingsHelper::digikamDBFile()
{
#ifdef Q_OS_WIN
    return "C:/Users/dmagro/Pictures/digikam4.db";
#else
    // return _settings->value("digikam_db_file", "/home/david/Imágenes/digikam4.db").toString();
    return "/home/david/tmp/imagenes/digikam4.db";
#endif
}
