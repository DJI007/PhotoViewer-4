#include "xmpmetadata.h"

#include <exiv2/xmp.hpp>
#include <QString>
#include <QDateTime>

#include "abstractmetadata.h"

#include <QDebug>
#include <exiv2/exiv2.hpp>

XMPMetadata::XMPMetadata(QString fileName)
{
    _fileData = Exiv2::ImageFactory::open(fileName.toUtf8().constData());
    _fileData->readMetadata ();
/*
    for (Exiv2::XmpData::const_iterator md = _fileData->xmpData().begin();
         md != _fileData->xmpData().end(); ++md) {
        std::cout << std::setfill(' ') << std::left
                  << std::setw(44)
                  << md->key() << " "
                  << std::setw(9) << std::setfill(' ') << std::left
                  << md->typeName() << " "
                  << std::dec << std::setw(3)
                  << std::setfill(' ') << std::right
                  << md->count() << "  "
                  << std::dec << md->value()
                  << std::endl;
    }
*/
}

bool XMPMetadata::hasKey(const char *keyName)
{
    Exiv2::XmpKey key(keyName);
    Exiv2::XmpData::iterator pos;

    pos = _fileData->xmpData().findKey(key);

    return (pos != _fileData->xmpData().end());
}


QString XMPMetadata::getString(const char *keyName)
{
    try {
        if (hasKey(keyName)) {
            return QString::fromUtf8 (_fileData->xmpData() [keyName].toString().c_str());
        }
        else {
            return "";
        }
    }
    catch (Exiv2::AnyError& e) {
        qDebug () << "Error reading string data: " << QString::fromUtf8(e.what());

        return "";
    }
}

long XMPMetadata::getLong(const char *keyName)
{
    try {
        if (hasKey(keyName)) {
            return _fileData->xmpData() [keyName].toLong();
        }
        else {
            return 0;
        }
    }
    catch (Exiv2::AnyError& e) {
        qDebug () << "Error reading long data: " << QString::fromUtf8(e.what());

        return -1;
    }
}

double XMPMetadata::getDoubleFromDegrees(const char *keyName)
{
    try {
        Exiv2::Rational r1;
        Exiv2::Rational r2;
        Exiv2::Rational r3;
        double result;

        if (hasKey(keyName)) {
            if (_fileData->xmpData() [keyName].count() == 3) {
                r1 = _fileData->xmpData() [keyName].toRational(0);
                r2 = _fileData->xmpData() [keyName].toRational(1);
                r3 = _fileData->xmpData() [keyName].toRational(2);

                result = ((double) r1.first) / ((double) r1.second);
                result += (((double) r2.first) / ((double) r2.second) / 60);
                result += (((double) r3.first) / ((double) r3.second)) / 3600;

                return result;
            }
            else {
                return 0;
            }
        }
        else {
            return -1;
        }
    }
    catch (Exiv2::AnyError& e) {
        qDebug () << "Error reading long data: " << QString::fromUtf8(e.what());

        return -1;
    }
}

double XMPMetadata::getDoubleFromRational(const char *keyName)
{
    try {
        Exiv2::Rational r;

        if (hasKey(keyName)) {
            r = _fileData->xmpData() [keyName].toRational(0);

            return ((double) r.first) / ((double) r.second);
        }
        else {
            return 0;
        }
    }
    catch (Exiv2::AnyError& e) {
        qDebug () << "Error reading long data: " << QString::fromUtf8(e.what());

        return -1;
    }
}

QDateTime XMPMetadata::pictureDate()
{
    QString strDate;

    strDate = getString("Xmp.video.MediaCreateDate");
    if (strDate != "") {
        QDateTime start;

        start = QDateTime::fromString("01/01/1904 00:00:00", "dd/MM/yyyy hh:mm:ss");

        return start.addSecs(strDate.toUInt());
    }
    else {
        QDateTime result;

        return result;
    }
}

int XMPMetadata::rating()
{
    long result;

    result = getLong ("Xmp.xmp.Rating");
    if (result == -1) {
        return 0;
    }
    else {
        return (int) result;
    }
}

void XMPMetadata::setRating(int value)
{
    _fileData->xmpData() ["Xmp.xmp.Rating"] = value;
    _fileData->writeMetadata();
}

int XMPMetadata::orientation()
{
    return 0;
}

void XMPMetadata::setOrientation (int value)
{
    Q_UNUSED (value);
}


bool XMPMetadata::hasGpsInfo()
{
    QString gpsCoordinates;

    gpsCoordinates = getString("Xmp.video.GPSCoordinates");

    return (gpsCoordinates != "");
}

double XMPMetadata::gpsLatitude()
{
    QString result;

    // TODO: Parse the result
    result = getString ("Xmp.video.GPSCoordinates");

    return 0;
}

QString XMPMetadata::gpsLatitudeRef ()
{
    QString result;

    // TODO: Parse the result
    result = getString ("Xmp.video.GPSCoordinates");

    return result;
}

double XMPMetadata::gpsLongitude()
{
    QString result;

    // TODO: Parse the result
    result = getString ("Xmp.video.GPSCoordinates");

    return 0;
}

QString XMPMetadata::gpsLongitudeRef ()
{
    QString result;

    // TODO: Parse the result
    result = getString ("Xmp.video.GPSCoordinates");

    return result;
}

double XMPMetadata::gpsAltitude()
{
    QString result;

    // TODO: Parse the result
    result = getString ("Xmp.video.GPSCoordinates");

    return 0;
}
