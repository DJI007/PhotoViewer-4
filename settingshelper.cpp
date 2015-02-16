#include "settingshelper.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>

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

bool SettingsHelper::showToolbar ()
{
    return _settings->value("show_toolbar", true).toBool();
}

void SettingsHelper::setShowToolbar (bool value)
{
    _settings->setValue("show_toolbar", value);
}

bool SettingsHelper::showLocationInformation ()
{
    return _settings->value("show_location_information", true).toBool();
}

void SettingsHelper::setShowLocationInformation (bool value)
{
    _settings->setValue("show_location_information", value);
}

bool SettingsHelper::showRating()
{
    return _settings->value("show_rating", true).toBool();
}

void SettingsHelper::setShowRating(bool value)
{
    _settings->setValue("show_rating", value);
}

bool SettingsHelper::showFileInformation ()
{
    return _settings->value("show_file_information", true).toBool();
}

void SettingsHelper::setShowFileInformation (bool value)
{
    _settings->setValue("show_file_information", value);
}

bool SettingsHelper::presentationShowLocationInformation ()
{
    return _settings->value("presentation_show_location_information", false).toBool();
}

void SettingsHelper::setPresentationShowLocationInformation (bool value)
{
    _settings->setValue("presentation_show_location_information", value);
}

bool SettingsHelper::presentationShowRating()
{
    return _settings->value("presentation_show_rating", false).toBool();
}

void SettingsHelper::setPresentationShowRating(bool value)
{
    _settings->setValue("presentation_show_rating", value);
}

bool SettingsHelper::presentationShowFileInformation ()
{
    return _settings->value("presentation_show_file_information", false).toBool();
}

void SettingsHelper::setPresentationShowFileInformation (bool value)
{
    _settings->setValue("presentation_show_file_information", value);
}

int SettingsHelper::presentationInterval ()
{
    return _settings->value("presentation_interval", 5).toInt();
}

void SettingsHelper::setPresentationInterval (int value)
{
    _settings->setValue("presentation_interval", value);
}

bool SettingsHelper::integrateDigikam ()
{
    return _settings->value("integrate_digikam", false).toBool();
}

void SettingsHelper::setIntegrateDigikam (bool value)
{
    _settings->setValue("integrate_digikam", value);
}

QString SettingsHelper::digikamDBFile ()
{
    return _settings->value("digikam_database_file", QDir::homePath () + "/digikam4.db").toString();
}

void SettingsHelper::setDigikamDBFile (QString value)
{
    _settings->setValue("digikam_database_file", value);
}

QByteArray SettingsHelper::mainWindowGeometry()
{
    return _settings->value("main_window_geometry").toByteArray();
}

void SettingsHelper::setMainWindowGeometry(QByteArray geometry)
{
    _settings->setValue("main_window_geometry", geometry);
}

QByteArray SettingsHelper::mainWindowState()
{
    return _settings->value("main_window_state").toByteArray();
}

void SettingsHelper::setMainWindowState(QByteArray state)
{
    _settings->setValue("main_window_state", state);
}






