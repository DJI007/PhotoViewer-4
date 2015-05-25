#ifndef DIGIKAMMETADATA_H
#define DIGIKAMMETADATA_H

#include <QString>
#include <QVariant>

#include "abstractmetadata.h"

#define SQL_SELECT "\
select i.id, \
       i.name, \
       ii.creationDate, \
       ii.rating, \
       ii.orientation, \
       substr(ip.latitude, -1) latitudeRef, \
       ip.latitudeNumber, \
       substr(ip.longitude, -1) longitudeRef, \
       ip.longitudeNumber, \
       ip.altitude \
from images i \
     inner join albums a on i.album = a.id \
     inner join albumroots ar on a.albumRoot = ar.id \
     left join imagepositions ip on i.id = ip.imageid \
     left join imageinformation ii on i.id = ii.imageid \
where i.name = ? and ar.identifier = ? and ar.specificPath = ? \
"

class DigikamMetadata : public AbstractMetadata
{
public:
    DigikamMetadata(QString fileName);
    ~DigikamMetadata();

    static bool isValidDBFile(QString fileName);

    QDateTime pictureDate ();
    int rating ();
    int orientation ();

    bool hasGpsInfo ();
    double gpsLatitude ();
    QString gpsLatitudeRef();
    double gpsLongitude();
    QString gpsLongitudeRef();
    double gpsAltitude ();

    void setRating (int value);
    void setOrientation (int value);

    bool existInDigikam ();

private:
    QString _fileName;

    int _id;
    QDateTime _creationDate;
    int _rating;
    int _orientation;
    QString _latitudeRef;
    double _latitude;
    QString _longitudeRef;
    double _longitude;
    double _altitude;

    bool _hasGpsInfo;
    bool _exist;

    void readFromDB ();
    void setImageInformationValue (QString column, QVariant value);

    int getIntValue (QVariant value, int defaultValue = 0);
    QString getStringValue (QVariant value, QString defaultValue = "");
    double getDoubleValue (QVariant value, double defaultValue = 0);
};

#endif // DIGIKAMMETADATA_H
