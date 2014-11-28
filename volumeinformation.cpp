#include "volumeinformation.h"

#include <QDebug>

#include <QFileInfoList>
#include <QDir>
#include <QDBusReply>
#include <QDBusObjectPath>
#include <QDBusMessage>
#include <QDBusConnection>
#include <QDBusInterface>

#ifdef Q_OS_WIN32

#include <qt_windows.h>

VolumeInformation::VolumeInformation(QString path)
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

#endif

#ifdef Q_OS_LINUX

extern "C" {
#include <stdio.h>
#include <mntent.h>
#include <blkid/blkid.h>
}

VolumeInformation::VolumeInformation (QString path)
{
    _srcPath = path;

    QFileInfoList fileinfolist = QDir(QString(QStringLiteral("/dev/disk/by-uuid/"))).entryInfoList(QDir::AllEntries | QDir::NoDot | QDir::NoDotDot);
    if (!fileinfolist.isEmpty()) {
        FILE *fsDescription;
        struct mntent entry;
        char buffer[512];
        QString currentPath;

        fsDescription = setmntent(_PATH_MOUNTED, "r");
        while ((getmntent_r(fsDescription, &entry, buffer, sizeof(buffer))) != NULL) {
            currentPath = QString::fromLatin1(entry.mnt_dir);
            if (path.startsWith(currentPath)) {
                int idx;

                idx = fileinfolist.indexOf(QString::fromLatin1(entry.mnt_fsname));
                if (idx != -1) {
                    _rootPath = currentPath;
                    _uuid = fileinfolist[idx].fileName();
                    if (currentPath != "/") {
                        break;
                    }
                }
            }
        }

        endmntent(fsDescription);
    }

/*
#if !defined(QT_NO_UDISKS)
    QDBusReply<QList<QDBusObjectPath> > reply = QDBusConnection::systemBus().call(
                QDBusMessage::createMethodCall(QString(QStringLiteral("org.freedesktop.UDisks")),
                                               QString(QStringLiteral("/org/freedesktop/UDisks")),
                                               QString(QStringLiteral("org.freedesktop.UDisks")),
                                               QString(QStringLiteral("EnumerateDevices"))));
    if (reply.isValid()) {
        QList<QDBusObjectPath> paths = reply.value();
        foreach (const QDBusObjectPath &currentPath, paths) {
            QDBusInterface interface(QString(QStringLiteral("org.freedesktop.UDisks")), currentPath.path(),
                                     QString(QStringLiteral("org.freedesktop.DBus.Properties")),
                                     QDBusConnection::systemBus());
            if (!interface.isValid())
                continue;
            QDBusReply<QVariant> reply = interface.call(QString(QStringLiteral("Get")),
                                                        QString(QStringLiteral("org.freedesktop.UDisks.Device")),
                                                        QString(QStringLiteral("DeviceMountPaths")));
            if (reply.isValid() && reply.value().toString() == path) {
                reply = interface.call(QString(QStringLiteral("Get")),
                                       QString(QStringLiteral("org.freedesktop.UDisks.Device")),
                                       QString(QStringLiteral("IdUuid")));
                if (reply.isValid())
                    return reply.value().toString();
            }
        }
    }
#endif // QT_NO_UDISKS
*/

}

#endif

QString VolumeInformation::uuid()
{
    return _uuid;
}

QString VolumeInformation::filePath()
{
    QFileInfo info (_srcPath);

    if (_rootPath == "/") {
        return info.absolutePath();
    }
    else {
        return info.absolutePath().remove(0, _rootPath.length());
    }
}

QString VolumeInformation::rootPath()
{
    return _rootPath;
}

QString VolumeInformation::fileName()
{
    return QFileInfo (_srcPath).fileName();
}




