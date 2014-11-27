#include "volumeinformation.h"

#include <QDebug>
#include <QString>
#include <QFileInfo>

#include <qt_windows.h>

QString VolumeInformation::volumeId(QString path)
{
    WCHAR szVolumeName[256] ;
    WCHAR szFileSystemName[256];
    DWORD dwSerialNumber = 0;
    DWORD dwMaxFileNameLength=256;
    DWORD dwFileSystemFlags=0;
    int pos;
    QString windowsPath;

    windowsPath = path.replace("/", "\\");

    qDebug () << windowsPath;
    pos = windowsPath.indexOf("\\");
    if (pos == -1) {
        return QObject::tr("Unknown");
    }

    qDebug () << "QFileInfo(path).baseName(): " << windowsPath.left(pos + 1);

    bool ret = GetVolumeInformation( (WCHAR *) windowsPath.left(pos + 1).utf16(),
                                     szVolumeName,
                                     256,
                                     &dwSerialNumber,
                                     &dwMaxFileNameLength,
                                     &dwFileSystemFlags,
                                     szFileSystemName,
                                     256);

    if (!ret) {
        return QString(QObject::tr("Unknown"));
    }
    else {
        return QString::number(dwSerialNumber, 16);
    }
}


