#include "exifmetadata.h"

#include <QDebug>
#include <exiv2/exiv2.hpp>

ExifMetadata::ExifMetadata(QString fileName)
{
    try {
        _imageData = Exiv2::ImageFactory::open(fileName.toUtf8().constData());
        _imageData->readMetadata ();

        _hasData = true;
    }
    catch (Exiv2::AnyError& e) {
        qDebug () << "Caught Exiv2 exception " << QString::fromUtf8 (e.what());

        _hasData = false;
    }
}

bool ExifMetadata::hasKey(const char *keyName)
{
    if (_hasData) {
        Exiv2::ExifKey key(keyName);
        Exiv2::ExifData::iterator pos;

        pos = _imageData->exifData ().findKey(key);

        return (pos != _imageData->exifData().end());
    }
    else {
        return false;
    }
}


QString ExifMetadata::getString(const char *keyName)
{
    try {
        if (hasKey(keyName)) {
            return QString::fromUtf8 (_imageData->exifData() [keyName].toString().c_str());
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

long ExifMetadata::getLong(const char *keyName)
{
    try {
        if (hasKey(keyName)) {
            return _imageData->exifData() [keyName].toLong();
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

double ExifMetadata::getDoubleFromDegrees(const char *keyName)
{
    try {
        Exiv2::Rational r1;
        Exiv2::Rational r2;
        Exiv2::Rational r3;
        double result;

        if (hasKey(keyName)) {
            if (_imageData->exifData() [keyName].count() == 3) {
                r1 = _imageData->exifData() [keyName].toRational(0);
                r2 = _imageData->exifData() [keyName].toRational(1);
                r3 = _imageData->exifData() [keyName].toRational(2);

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

double ExifMetadata::getDoubleFromRational(const char *keyName)
{
    try {
        Exiv2::Rational r;

        if (hasKey(keyName)) {
            r = _imageData->exifData() [keyName].toRational(0);

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

QDateTime ExifMetadata::pictureDate()
{
    QString strDate;

    // strDate = QString::fromUtf8 (_imageData->exifData() ["Exif.Photo.DateTimeOriginal"].toString().c_str());
    strDate = getString("Exif.Photo.DateTimeOriginal");

    if (strDate != "") {
        return QDateTime::fromString(strDate, "yyyy:MM:dd hh:mm:ss");
    }
    else {
        return QDateTime::fromTime_t(0);
    }
}

int ExifMetadata::rating()
{
    long result;

    result = getLong ("Exif.Image.Rating");
    if (result == -1) {
        result = getLong ("Exif.Image.RatingPercent");

        if (result != -1) {
            result = (long) ((result * 5) / 100);
        }
    }

    if (result == -1)
        return 0;

    return (int) result;
}

void ExifMetadata::setRating(int value)
{
    _imageData->exifData() ["Exif.Image.Rating"] = value;
    _imageData->exifData() ["Exif.Image.RatingPercent"] = (100 * value) / 5;
    _imageData->writeMetadata();
}

int ExifMetadata::orientation()
{
    int result;

    result = (int) getLong ("Exif.Image.Orientation");
    if (result == -1) {
        return 1;
    }
    else {
        return result;
    }
}

void ExifMetadata::setOrientation (int value)
{
    _imageData->exifData() ["Exif.Image.Orientation"] = value;
    _imageData->writeMetadata();
}

bool ExifMetadata::hasGpsInfo()
{
    QString gpsVersionID;

    gpsVersionID = getString("Exif.GPSInfo.GPSVersionID");

    return (gpsVersionID != "");
}

double ExifMetadata::gpsLatitude()
{
    double result;

    result = getDoubleFromDegrees ("Exif.GPSInfo.GPSLatitude");
    if (gpsLatitudeRef() == "N")
        return result;
    else
        return -result;
}

QString ExifMetadata::gpsLatitudeRef ()
{
    return getString ("Exif.GPSInfo.GPSLatitudeRef");
}

double ExifMetadata::gpsLongitude()
{
    double result;

    result = getDoubleFromDegrees("Exif.GPSInfo.GPSLongitude");
    if (gpsLongitudeRef() == "E")
        return result;
    else
        return -result;
}

QString ExifMetadata::gpsLongitudeRef ()
{
    return getString ("Exif.GPSInfo.GPSLongitudeRef");
}

double ExifMetadata::gpsAltitude()
{
    double result;

    result = getDoubleFromRational ("Exif.GPSInfo.GPSAltitude");
    if (getLong("Exif.GPSInfo.GPSAltitudeRef") == 0)
        return result;
    else
        return -result;

}
