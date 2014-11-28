#ifndef VOLUMEINFORMATION_H
#define VOLUMEINFORMATION_H

#include <QString>

class VolumeInformation
{
public:
    explicit VolumeInformation (QString path);

    QString uuid ();
    QString rootPath ();
    QString filePath ();
    QString fileName ();

    QString volumeUri () { return QString("volumeid:?uuid=%1").arg(_uuid); }

private:
    QString _srcPath;
    QString _uuid;
    QString _rootPath;
};

#endif // VOLUMEINFORMATION_H
