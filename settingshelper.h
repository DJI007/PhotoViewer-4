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
    static SettingsHelper& instance()
            {
                static SettingsHelper instance; // Guaranteed to be destroyed.
                                                // Instantiated on first use.
                return instance;
            }

    QString lastDirectory ();
    void setLastDirectory (QString);

    int lastFileIndex ();
    void setLastFileIndex (int value);

    int volume();
    void setVolume(int value);

    QString digikamDBFile ();
};

#endif // SETTINGSHELPER_H
