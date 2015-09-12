#ifndef XMPMETADATA_H
#define XMPMETADATA_H

#include <exiv2/exiv2.hpp>
#include <QString>
#include <QDateTime>

#include "abstractmetadata.h"

class XMPMetadata : public AbstractMetadata
{
public:
    explicit XMPMetadata(QString fileName);

    QDateTime pictureDate ();
    int rating ();
    quint16 orientation();

    bool hasGpsInfo();
    double gpsLatitude ();
    QString gpsLatitudeRef();
    double gpsLongitude();
    QString gpsLongitudeRef();
    double gpsAltitude ();

    void setRating (int value);
    void setOrientation (quint16 value);

private:
    Exiv2::Image::AutoPtr _fileData;
    QString _fileName;

    QString getString (const char *keyName);
    long getLong (const char *keyName);
    double getDoubleFromDegrees(const char *keyName);
    double getDoubleFromRational(const char *keyName);

    bool hasKey (const char *keyName);
};


#endif // XMPMETADATA_H
