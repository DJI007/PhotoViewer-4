#ifndef SETTINGSHELPER_H
#define SETTINGSHELPER_H


#include <QSettings>

class SettingsHelper
{
private:
    QSettings *_settings;

    SettingsHelper();
    ~SettingsHelper ();
    SettingsHelper (SettingsHelper const&);
    void operator=(SettingsHelper const&);

public:
    static SettingsHelper& getInstance()
            {
                static SettingsHelper instance; // Guaranteed to be destroyed.
                                                // Instantiated on first use.
                return instance;
            }

    QString lastDirectory ();
    void setLastDirectory (QString);
};

#endif // SETTINGSHELPER_H
