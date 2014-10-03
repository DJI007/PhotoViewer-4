#include "exifmetadata.h"

#include <QDebug>
#include <exiv2/exiv2.hpp>

ExifMetadata::ExifMetadata(QString fileName)
{
    try {
        _imageData = Exiv2::ImageFactory::open(fileName.toUtf8().constData());
        _imageData->readMetadata ();
    }
    catch (Exiv2::AnyError& e) {
        qDebug () << "Caught Exiv2 exception " << QString::fromUtf8 (e.what());
    }
}


QString ExifMetadata::getString(const char *tagName)
{
    try {
        return QString::fromUtf8 (_imageData->exifData() [tagName].toString().c_str());
    }
    catch (Exiv2::AnyError& e) {
        qDebug () << "Error reading string data: " << QString::fromUtf8(e.what());

        return "";
    }
}

long ExifMetadata::getLong(const char *tagName)
{
    try {
        return _imageData->exifData() [tagName].toLong();
    }
    catch (Exiv2::AnyError& e) {
        qDebug () << "Error reading long data: " << QString::fromUtf8(e.what());

        return -1;
    }
}

double ExifMetadata::getDoubleFromDegrees(const char *tagName)
{
    try {
        Exiv2::Rational r1;
        Exiv2::Rational r2;
        Exiv2::Rational r3;
        double result;

        if (_imageData->exifData() [tagName].count() == 3) {
            r1 = _imageData->exifData() [tagName].toRational(0);
            r2 = _imageData->exifData() [tagName].toRational(1);
            r3 = _imageData->exifData() [tagName].toRational(2);

            result = ((double) r1.first) / ((double) r1.second);
            result += (((double) r2.first) / ((double) r2.second) / 60);
            result += (((double) r3.first) / ((double) r3.second)) / 3600;

            return result;
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

double ExifMetadata::getDoubleFromRational(const char *tagName)
{
    try {
        Exiv2::Rational r;

        r = _imageData->exifData() [tagName].toRational(0);

        return ((double) r.first) / ((double) r.second);
    }
    catch (Exiv2::AnyError& e) {
        qDebug () << "Error reading long data: " << QString::fromUtf8(e.what());

        return -1;
    }
}

QString ExifMetadata::manufacturer()
{
    return getString ("Exif.Image.Make");
}

QDateTime ExifMetadata::pictureDate()
{
    QString strDate;

    strDate = QString::fromUtf8 (_imageData->exifData() ["Exif.Photo.DateTimeOriginal"].toString().c_str());

    return QDateTime::fromString(strDate, "yyyy:MM:dd hh:mm:ss");
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
        result = 1;

        // If don't save default value, when save metadata the exif
        // data saved is corrupted
        _imageData->exifData() ["Exif.Image.Orientation"] = 1;
    }

    return result;
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
