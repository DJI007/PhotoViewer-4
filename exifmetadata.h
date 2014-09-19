#ifndef EXIFMETADATA_H
#define EXIFMETADATA_H

#include <exiv2/exiv2.hpp>
#include <QString>
#include <QDateTime>

class ExifMetadata
{
public:
    explicit ExifMetadata();

    void loadData (QString);

    QString manufacturer ();
    QDateTime pictureDate ();
    int rating ();
    int orientation ();
    QString gpsTag ();
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
