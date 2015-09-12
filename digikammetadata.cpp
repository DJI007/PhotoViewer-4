#include "digikammetadata.h"

#include <QTextStream>
#include <QFileInfo>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include <QDebug>

#include "settingshelper.h"
#include "volumeinformation.h"

DigikamMetadata::DigikamMetadata(QString fileName)
{
    _fileName = fileName;

    readFromDB ();

    if (!_exist) {
        _id = -1;
        _creationDate = QFileInfo(fileName).created();
        _rating = 0;
        _orientation = 0;

        _hasGpsInfo = false;
        _latitudeRef = "";
        _latitude = 0;
        _longitudeRef = "";
        _longitude = 0;
        _altitude = 0;
    }
}

DigikamMetadata::~DigikamMetadata()
{
}

void DigikamMetadata::setImageInformationValue(QString column, QVariant value)
{
    if (_exist) {
        QString sql;
        QSqlQuery q;

        QTextStream(&sql) << "update imageinformation set " << column << " = ? where imageid = ?";

        q.prepare(sql);
        q.addBindValue(value);
        q.addBindValue(_id);
        q.exec();
    }
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

quint16 DigikamMetadata::orientation()
{
    return _orientation;
}

void DigikamMetadata::setOrientation (quint16 value)
{
    _orientation = value;
    setImageInformationValue("orientation", value);
}

bool DigikamMetadata::hasGpsInfo()
{
    return _hasGpsInfo;
}

double DigikamMetadata::gpsLatitude()
{
    return _latitude;
}

QString DigikamMetadata::gpsLatitudeRef ()
{
    return _latitudeRef;
}

double DigikamMetadata::gpsLongitude()
{
    return _longitude;
}

QString DigikamMetadata::gpsLongitudeRef ()
{
    return _longitudeRef;
}

double DigikamMetadata::gpsAltitude()
{
    return _altitude;
}

void DigikamMetadata::readFromDB()
{
    QString dbName;

    dbName = SettingsHelper::instance().digikamDBFile();

    if (!QSqlDatabase::database().isValid()) {
        QSqlDatabase db;

        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(dbName);
    }

    if (QSqlDatabase::database().open()) {
        QSqlQuery q;
        VolumeInformation volInfo (_fileName);

        // qDebug () << volInfo.fileName() << "-.-" << volInfo.volumeUri() << "-.-" << volInfo.filePath();

        q.prepare(SQL_SELECT);
        q.addBindValue(volInfo.fileName());
        q.addBindValue(volInfo.volumeUri());
        q.addBindValue(volInfo.filePath());
        q.exec();

        if (q.next()) {
            _id = q.value("id").toInt();
            _creationDate = q.value("creationDate").toDateTime();

            _rating = getIntValue(q.value("rating"));
            _orientation = getUInt16Value(q.value("orientation"), 1);

            if (q.value("latitudeNumber").isNull()) {
                _hasGpsInfo = false;
            }
            else {
                _hasGpsInfo = true;
                _latitudeRef = getStringValue(q.value("latitudeRef"));
                _latitude = getDoubleValue(q.value("latitudeNumber"));
                _longitudeRef = getStringValue(q.value("longitudeRef"));
                _longitude = getDoubleValue(q.value("longitudeNumber"));
                _altitude = getDoubleValue(q.value("altitude"));
            }

            // qDebug () << "Reading digikam info: " << _rating << ", " << _orientation;

            _exist = true;
        }
        else {
            // qDebug () << "Doesn't exist in digikam";

            _exist = false;
        }
    }
    else {
        // qDebug () << "Can't open database";
        _exist = false;
    }
}

int DigikamMetadata::getIntValue(QVariant value, int defaultValue)
{
    if (value.isNull()) {
        return defaultValue;
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

quint16 DigikamMetadata::getUInt16Value(QVariant value, quint16 defaultValue)
{
    if (value.isNull()) {
        return defaultValue;
    }
    else {
        quint16 result;

        result = value.toUInt();

        return result;
    }
}

QString DigikamMetadata::getStringValue(QVariant value, QString defaultValue)
{
    if (value.isNull()) {
        return defaultValue;
    }
    else {
        return value.toString();
    }
}

double DigikamMetadata::getDoubleValue(QVariant value, double defaultValue)
{
    if (value.isNull()) {
        return defaultValue;
    }
    else {
        return value.toDouble();
    }
}

bool DigikamMetadata::isValidDBFile(QString fileName)
{
    bool result;

    result = false;

    // Scope to free database objects and all removeDatabase safely
    {
        QSqlDatabase db;

        db = QSqlDatabase::addDatabase("QSQLITE", "test_connection");
        db.setDatabaseName(fileName);

        if (db.open()) {
            result = db.tables().contains(QLatin1String("Images"));
        }

        db.close();
    }

    QSqlDatabase::removeDatabase("test_connection");

    return result;
}

bool DigikamMetadata::existInDigikam()
{
    return _exist;
}
