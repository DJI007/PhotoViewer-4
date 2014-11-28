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
    void setOrientation (int value);

private:
    QString _fileName;
    Exiv2::Image::AutoPtr _imageData;
    bool _hasData;

    QString getString (const char *keyName);
    long getLong (const char *keyName);
    double getDoubleFromDegrees(const char *keyName);
    double getDoubleFromRational(const char *keyName);

    bool hasKey (const char *keyName);
};

#endif // EXIFMETADATA_H
