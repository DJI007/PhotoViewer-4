#include "sqlitemetadata.h"

#include <QTextStream>
#include <QFileInfo>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include <QDebug>

SQLiteMetadata::SQLiteMetadata(QString fileName)
{
    _fileName = fileName;

    initDB ();
}

SQLiteMetadata::~SQLiteMetadata()
{
}

void SQLiteMetadata::setValue(QString column, QVariant value)
{
    QString sql;
    QSqlQuery q;

    QTextStream(&sql) << "update file_metadata set " << column << " = ? where file_name = ?";

    q.prepare(sql);
    q.addBindValue(value);
    q.addBindValue(QFileInfo(_fileName).fileName());
    q.exec();
}

bool SQLiteMetadata::exist()
{
    QSqlQuery q;

    q.prepare("select * from file_metadata where file_name = ?");
    q.addBindValue(QFileInfo(_fileName).fileName());
    q.exec();

    return q.next();
}

QVariant SQLiteMetadata::getValue(QString column)
{
    QString sql;
    QSqlQuery q;
    QVariant result;

    QTextStream(&sql) << "select " << column << " from file_metadata where file_name = ?";

    q.prepare(sql);
    q.addBindValue(QFileInfo(_fileName).fileName());
    q.exec();

    if (q.next()) {
        result = q.value(0);
    }
    else {
        result = QVariant();
    }

    return result;
}

int SQLiteMetadata::getInt(QString column)
{
    QVariant result;

    result = getValue (column);
    if (result.isValid()) {
        return result.toInt();
    }
    else {
        return 0;
    }
}

quint16 SQLiteMetadata::getUInt16(QString column)
{
    QVariant result;

    result = getValue (column);
    if (result.isValid()) {
        return result.toUInt();
    }
    else {
        return 0;
    }
}

QString SQLiteMetadata::getString(QString column)
{
    QVariant result;

    result = getValue (column);
    if (result.isValid()) {
        return result.toString();
    }
    else {
        return "";
    }
}

QDateTime SQLiteMetadata::pictureDate()
{
    QDateTime result;

    return result;
}

int SQLiteMetadata::rating()
{
    return getInt ("rating");
}

void SQLiteMetadata::setRating(int value)
{
    setValue("rating", value);
}

quint16 SQLiteMetadata::orientation()
{
    return getUInt16("orientation");
}

void SQLiteMetadata::setOrientation (quint16 value)
{
    setValue ("orientation", value);
}

bool SQLiteMetadata::hasGpsInfo()
{
    return false;
}

double SQLiteMetadata::gpsLatitude()
{
    return 0;
}

QString SQLiteMetadata::gpsLatitudeRef ()
{
    return "";
}

double SQLiteMetadata::gpsLongitude()
{
    return 0;
}

QString SQLiteMetadata::gpsLongitudeRef ()
{
    return "";
}

double SQLiteMetadata::gpsAltitude()
{
    return 0;
}

void SQLiteMetadata::initDB ()
{
    QString dbName;
    QFileInfo info(_fileName);
    QSqlDatabase db;

    dbName = info.absolutePath();
    dbName.append("/");
    dbName.append("photoviewer.db");

    if (!QSqlDatabase::database().isValid()) {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(dbName);

        db.open();
    }

    QStringList tables = db.tables();
    QSqlQuery q;

    if (!tables.contains("file_metadata", Qt::CaseInsensitive)) {
        q.exec(QLatin1String("create table file_metadata (id integer primary key, \
                                                          file_name text, \
                                                          rating integer, \
                                                          orientation integer)"));

    }

    if (!exist()) {
        q.prepare(QLatin1String("insert into file_metadata (file_name) values (?)"));
        q.addBindValue(info.fileName());
        q.exec();
    }
}

