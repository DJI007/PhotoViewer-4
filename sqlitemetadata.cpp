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
    qDebug () << "Destroying SQLiteMetadata";
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

QDateTime SQLiteMetadata::pictureDate()
{
    QDateTime result;

    return result;
}

int SQLiteMetadata::rating()
{
    QVariant result;

    qDebug () << "getRating";

    result = getValue("rating");
    if (result.isValid()) {
        qDebug () << "Rating: " << result.toInt();

        return result.toInt();
    }
    else {
        qDebug () << "no rating";
        return 0;
    }
}

void SQLiteMetadata::setRating(int value)
{
    qDebug () << "setRating: " << value;
    setValue("rating", value);
}

int SQLiteMetadata::orientation()
{
    return 0;
}

void SQLiteMetadata::setOrientation (int value)
{
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

        QStringList tables = db.tables();
        QSqlQuery q;

        if (!tables.contains("file_metadata", Qt::CaseInsensitive)) {
            q.exec(QLatin1String("create table file_metadata (id integer primary key, \
                                                              file_name text, \
                                                              rating integer, \
                                                              orientation integer)"));

        }

        q.prepare(QLatin1String("insert into file_metadata (file_name) values (?)"));
        q.addBindValue(info.fileName());
        q.exec();
    }
}

