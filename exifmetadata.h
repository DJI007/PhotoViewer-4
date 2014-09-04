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
    int getOrientation ();

private:
    Exiv2::Image::AutoPtr _imageData;

    QString getString (const char *tagName);
    long getLong (const char *tagName);
};

#endif // EXIFMETADATA_H
