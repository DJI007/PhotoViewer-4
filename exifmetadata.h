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

    QString getManufacturer ();
    QDateTime getPictureDate ();
    int getRating ();

private:
    Exiv2::Image::AutoPtr _imageData;

};

#endif // EXIFMETADATA_H
