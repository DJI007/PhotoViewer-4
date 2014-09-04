#include "exifmetadata.h"

#include <QDebug>
#include <exiv2/exiv2.hpp>

ExifMetadata::ExifMetadata()
{
}

void ExifMetadata::loadData(QString fileName)
{
    _imageData = Exiv2::ImageFactory::open(fileName.toUtf8().constData());
    _imageData->readMetadata ();
}

QString ExifMetadata::getString(const char *tagName)
{
    return QString::fromUtf8 (_imageData->exifData() [tagName].toString().c_str());
}

long ExifMetadata::getLong(const char *tagName)
{
    return _imageData->exifData() [tagName].toLong();
}

QString ExifMetadata::getManufacturer()
{
    return getString ("Exif.Image.Make");
}

QDateTime ExifMetadata::getPictureDate()
{
    QString strDate;

    strDate = QString::fromUtf8 (_imageData->exifData() ["Exif.Photo.DateTimeOriginal"].toString().c_str());

    return QDateTime::fromString(strDate, "yyyy:MM:dd hh:mm:ss");
}

int ExifMetadata::getRating()
{
    long result;

    result = getLong ("Exif.Image.Rating");
    if (result == -1) {
        result = getLong ("Exif.Image.RatingPercent");

        if (result != -1) {
            result = (long) ((result * 4) / 100);
        }
    }

    if (result == -1)
        return 0;

    return (int) result;
}

int ExifMetadata::getOrientation()
{
    return (int) getLong ("Exif.Image.Orientation");
}
