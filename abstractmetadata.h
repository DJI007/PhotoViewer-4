#ifndef ABSTRACTMETADATA_H
#define ABSTRACTMETADATA_H

#include <QString>
#include <QDateTime>

class AbstractMetadata
{
public:
    virtual QString manufacturer () = 0;
    virtual QDateTime pictureDate () = 0;
    virtual int rating () = 0;
    virtual int orientation () = 0;

    virtual bool hasGpsInfo () = 0;
    virtual double gpsLatitude () = 0;
    virtual QString gpsLatitudeRef() = 0;
    virtual double gpsLongitude() = 0;
    virtual QString gpsLongitudeRef() = 0;
    virtual double gpsAltitude () = 0;

    virtual void setRating (int value) = 0;
};

#endif // ABSTRACTMETADATA_H
