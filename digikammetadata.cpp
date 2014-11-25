#include "digikammetadata.h"

#include <QTextStream>
#include <QFileInfo>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include <QDebug>

#include "settingshelper.h"

/* Digikam select:
 *   select i.id,
 *          i.name,
 *          ii.creationDate,
 *          ii.rating,
 *          ii.orientation,
 *          substr(ip.latitude, -1) latitudeRef,
 *          ip.latitudeNumber,
 *          substr(ip.longitude, -1) longitudeRef,
 *          ip.longitudeNumber,
 *          ip.altitude
 *   from images i
 *        left join imagepositions ip on i.id = ip.imageid
 *        left join imageinformation ii on i.id = ii.imageid
 *   where i.name = ?
 */

DigikamMetadata::DigikamMetadata(QString fileName)
{
    _fileName = fileName;

    readFromDB ();
}

DigikamMetadata::~DigikamMetadata()
{
}

void DigikamMetadata::setImageInformationValue(QString column, QVariant value)
{
    QString sql;
    QSqlQuery q;

    QTextStream(&sql) << "update imageinformation set " << column << " = ? where imageid = ?";

    q.prepare(sql);
    q.addBindValue(value);
    q.addBindValue(_id);
    q.exec();
}


QDateTime DigikamMetadata::pictureDate()
{
    return _creationDate;
}

int DigikamMetadata::rating()
{
    return _rating;
}

void DigikamMetadata::setRating(int value)
{
    _rating = value;
    setImageInformationValue("rating", value);
}

int DigikamMetadata::orientation()
{
    return _orientation;
}

void DigikamMetadata::setOrientation (int value)
{
    _orientation = value;
    setImageInformationValue("orientation", value);
}

bool DigikamMetadata::hasGpsInfo()
{
    return false;
}

double DigikamMetadata::gpsLatitude()
{
    return 0;
}

QString DigikamMetadata::gpsLatitudeRef ()
{
    return "";
}

double DigikamMetadata::gpsLongitude()
{
    return 0;
}

QString DigikamMetadata::gpsLongitudeRef ()
{
    return "";
}

double DigikamMetadata::gpsAltitude()
{
    return 0;
}

void DigikamMetadata::readFromDB()
{
    QString dbName;
    QSqlDatabase db;

    dbName = SettingsHelper::instance().digikamDBFile();

    if (!QSqlDatabase::database().isValid()) {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(dbName);
    }

    db.open();
    if (db.isOpen()) {
        QSqlQuery q;

        q.prepare(SQL_SELECT);
        q.addBindValue(QFileInfo(_fileName).fileName());
        q.exec();

        if (q.next()) {
            _id = q.value("id").toInt();
            _creationDate = q.value("creationDate").toDateTime();

            _rating = getIntValue(q.value("rating"));
            _orientation = getIntValue(q.value("orientation"));
            _latitudeRef = getStringValue(q.value("latitudeRef"));
            _latitude = getDoubleValue(q.value("latitudeNumber"));
            _longitudeRef = getStringValue(q.value("longitudeRef"));
            _longitude = getDoubleValue(q.value("longitudeNumber"));
            _altitude = getDoubleValue(q.value("altitude"));

            _exist = true;
        }
        else {
            _exist = false;
        }
    }
    else {
        _exist = false;
    }
}

int DigikamMetadata::getIntValue(QVariant value)
{
    if (value.isNull()) {
        return 0;
    }
    else {
        int result;

        result = value.toInt();
        if (result == -1) {
            return 0;
        }
        else {
            return result;
        }
    }
}

QString DigikamMetadata::getStringValue(QVariant value)
{
    if (value.isNull()) {
        return "";
    }
    else {
        return value.toString();
    }
}

double DigikamMetadata::getDoubleValue(QVariant value)
{
    if (value.isNull()) {
        return 0;
    }
    else {
        return value.toDouble();
    }
}

