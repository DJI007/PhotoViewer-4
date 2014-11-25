#ifndef DIGIKAMMETADATA_H
#define DIGIKAMMETADATA_H

#include <QString>

#include "abstractmetadata.h"

#define SQL_SELECT "select i.id, \
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
                         left join imagepositions ip on i.id = ip.imageid \
                         left join imageinformation ii on i.id = ii.imageid \
                    where i.name = ?"

class DigikamMetadata : public AbstractMetadata
{
public:
    DigikamMetadata(QString fileName);
    ~DigikamMetadata();

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

private:
    int _id;
    QString _fileName;
    QDateTime _creationDate;
    int _rating;
    int _orientation;
    QString _latitudeRef;
    double _latitude;
    QString _longitudeRef;
    double _longitude;
    double _altitude;
    bool _exist;

    void readFromDB ();
    void setImageInformationValue (QString column, QVariant value);

    int getIntValue (QVariant value);
    QString getStringValue (QVariant value);
    double getDoubleValue (QVariant value);
};

#endif // DIGIKAMMETADATA_H
