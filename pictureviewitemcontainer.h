#ifndef PICTUREVIEWITEMCONTAINER_H
#define PICTUREVIEWITEMCONTAINER_H

#include <QObject>

#include <QGeoServiceProvider>
#include <QGeoCodingManager>

#include "pictureviewitem.h"
#include "objectpixmapitem.h"
#include "clickabletextitem.h"
#include "animatedtextitem.h"

class PictureViewItemContainer : public QObject
{
    Q_OBJECT

public:
    explicit PictureViewItemContainer(QString fileName, QObject *parent = 0);
    ~PictureViewItemContainer ();

    PictureViewItem *item ();
    QGraphicsItem *graphicsItem ();

    void load ();
    void resize ();

    void setInfoVisible (bool visible);

    void setRating (int value);

    double latitude ();
    double longitude ();

signals:
    void itemLoaded ();
    void requestMapWindow (double latitude, double longitude, double altitude);

public slots:
    void on_itemLoaded();

    void on_beginItemAnimation ();
    void on_endItemAnimation ();

    void on_reverseGeocode_error(QGeoCodeReply::Error error, const QString &errorString);
    void on_reverseGeocode_finished();

    void on_geoInfo_leftMousePressed ();

private:
    QString _fileName;
    PictureViewItem *_item;

    QGeoServiceProvider *_geoProvider;
    QGeoCodingManager  *_geoManager;
    QGeoCodeReply *_reverseGeocodeReply;

    AnimatedTextItem *_info;
    ClickableTextItem *_geoInfo;
    QGraphicsItemGroup *_rating;

    AnimatedTextItem *createInfo();
    ClickableTextItem *createGeoInfo ();
    QGraphicsItemGroup *createRating ();
    ObjectPixmapItem *createStar (bool, int left, int top);

    void setInfoRatingPosition();
};

#endif // PICTUREVIEWITEMCONTAINER_H