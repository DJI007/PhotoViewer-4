#include "exifmetadata.h"

#include <QDebug>
#include <exiv2/exiv2.hpp>

ExifMetadata::ExifMetadata()
{
}

void ExifMetadata::loadData(QString fileName)
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
