#ifndef EXIFMETADATA_H
#define EXIFMETADATA_H

#include <exiv2/exiv2.hpp>
#include <QString>
#include <QDateTime>

#include "abstractmetadata.h"

class ExifMetadata : public AbstractMetadata
{
public:
    explicit ExifMetadata(QString fileName);

    QString manufacturer ();

    QDateTime pictureDate ();
    int rating ();
    int orientation ();

    bool hasGpsInfo();
    double gpsLatitude ();
    QString gpsLatitudeRef();
    double gpsLongitude();
    QString gpsLongitudeRef();
    double gpsAltitude ();

    void setRating (int value);

private:
    Exiv2::Image::AutoPtr _imageData;

    QString getString (const char *tagName);
    long getLong (const char *tagName);
    double getDoubleFromDegrees(const char *tagName);
    double getDoubleFromRational(const char *tagName);
};

#endif // EXIFMETADATA_H
