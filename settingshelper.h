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

    // Settings dialog

    bool showToolbar ();
    void setShowToolbar (bool value);

    bool showLocationInformation ();
    void setShowLocationInformation (bool value);

    bool showRating();
    void setShowRating(bool value);

    bool showFileInformation ();
    void setShowFileInformation (bool value);

    bool presentationShowLocationInformation ();
    void setPresentationShowLocationInformation (bool value);

    bool presentationShowRating();
    void setPresentationShowRating(bool value);

    bool presentationShowFileInformation ();
    void setPresentationShowFileInformation (bool value);

    int presentationInterval ();
    void setPresentationInterval (int value);

    bool integrateDigikam ();
    void setIntegrateDigikam (bool value);

    QString digikamDBFile ();
    void setDigikamDBFile (QString value);
};

#endif // SETTINGSHELPER_H
