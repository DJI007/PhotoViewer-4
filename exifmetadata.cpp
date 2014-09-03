#include "exifmetadata.h"

#include <QDebug>

ExifMetadata::ExifMetadata()
{
}

void ExifMetadata::loadData(QString fileName)
{
    _imageData = Exiv2::ImageFactory::open(fileName.toUtf8().constData());
    _imageData->readMetadata ();
}


QString ExifMetadata::getManufacturer()
{
    return QString::fromUtf8 (_imageData->exifData() ["Exif.Image.Make"].toString().c_str());
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

    result = _imageData->exifData() ["Exif.Image.Rating"].toLong();
    if (result == -1) {
        result = _imageData->exifData() ["Exif.Image.RatingPercent"].toLong();

        if (result != -1) {
            result = (long) ((result * 4) / 100);
        }
    }

    if (result == -1)
        return 0;

    return (int) result;
}
