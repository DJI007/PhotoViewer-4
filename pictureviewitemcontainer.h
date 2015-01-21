#ifndef PICTUREVIEWITEMCONTAINER_H
#define PICTUREVIEWITEMCONTAINER_H

#include <QObject>

#include <QGeoServiceProvider>
#include <QGeoCodingManager>
#include <QAnimationGroup>

#include "pictureviewitem.h"
#include "objectpixmapitem.h"
#include "clickabletextitem.h"
#include "animatedtextitem.h"

//#include <QtAV/AVPlayer.h>

class PictureViewItemContainer : public QObject,
        public QGraphicsRectItem,
        public AnimatedItem
{
    Q_OBJECT

    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
    Q_PROPERTY(qreal itemRotation READ itemRotation WRITE setItemRotation)
    Q_PROPERTY(qreal itemScale READ itemScale WRITE setItemScale)

public:
    explicit PictureViewItemContainer(QString fileName, QObject *parent = 0);
    ~PictureViewItemContainer ();

    PictureViewItem *item ();
    QGraphicsItem *graphicsItem ();

    void load ();
    void resize ();

    void showGeoInfo ();
    void showFileInfo ();
    void showRating ();

    void hideGeoInfo ();
    void hideFileInfo ();
    void hideRating ();

    void setRating (int value);

    double latitude ();
    double longitude ();

    void setShowTime(int time);

    void rotatePictureLeft ();
    void rotatePictureRight ();

    qreal itemRotation ();
    qreal itemScale ();

    void setItemRotation (qreal angle);
    void setItemScale (qreal scale);

    void updateShowInformation ();

signals:
    void itemLoaded ();
    void showTimeEnded ();
    void requestMapWindow (double latitude, double longitude, double altitude);

    void beginRotateAnimation ();
    void endRotateAnimation ();

public slots:
    void setItemLoaded();

    void beginItemAnimationIn ();
    void endItemAnimationIn ();
    void beginItemAnimationOut ();
    void endItemAnimationOut ();

    void setShowTimeEnded();

private slots:
    void on_endRotateAnimation ();
    void on_reverseGeocode_error(QGeoCodeReply::Error error, const QString &errorString);
    void on_reverseGeocode_finished();
    void on_geoInfo_leftMousePressed ();

    void on_hideFileInfoAnimationEnd ();
    void on_hideGeoInfoAnimationEnd ();
    void on_hideRatingAnimationEnd ();

private:
    QString _fileName;
    PictureViewItem *_item;

    QGeoServiceProvider *_geoProvider;
    QGeoCodingManager  *_geoManager;
    QGeoCodeReply *_reverseGeocodeReply;

    bool _rotating;

    AnimatedTextItem *_fileInfo;
    ClickableTextItem *_geoInfo;
    QGraphicsItemGroup *_rating;

    bool _fileInfoVisible;
    bool _geoInfoVisible;
    bool _ratingVisible;

    AnimatedTextItem *createFileInfo();
    ClickableTextItem *createGeoInfo ();
    QGraphicsItemGroup *createRating ();
    ObjectPixmapItem *createStar (bool, int left, int top);

    void setGeoInfoPosition ();
    void setFileInfoPosition ();
    void setRatingPosition ();

    QAbstractAnimation *getAnimation (AnimatedItem *item, bool in, int width);

    // void setInfoRatingPosition();

    void doRotation (int angle);

    void initGeoManager ();
};

#endif // PICTUREVIEWITEMCONTAINER_H
