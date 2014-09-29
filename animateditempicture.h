#ifndef ANIMATEDITEMPICTURE_H
#define ANIMATEDITEMPICTURE_H

#include <QGraphicsPixmapItem>
#include <QString>
#include <QGeoServiceProvider>
#include <QGeoCodingManager>
#include <QGraphicsTextItem>

#include "exifmetadata.h"
#include "clickableitemtext.h"
#include "animateditem.h"

class AnimatedItemPicture: public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    explicit AnimatedItemPicture(const QPixmap& pixmap, QObject* parent = 0);
    explicit AnimatedItemPicture(const QString fileName, QObject* parent = 0);
    ~AnimatedItemPicture ();

    QString toString ();

    void load ();
    void resize ();
    void setRating(int rating);
    double latitude ();
    double longitude ();

    void setInfoVisible (bool visible);

    QPointF animationPos ();
    qreal animationOpacity ();
    qreal animationRotation ();
    qreal animationScale ();

    void setAnimationPos (QPointF);
    void setAnimationOpacity (qreal);
    void setAnimationRotation (qreal);
    void setAnimationScale (qreal);

signals:

public slots:
    void on_reverseGeocode_error(QGeoCodeReply::Error error, const QString &errorString);
    void on_reverseGeocode_finished();
    void on_geoInfo_leftMousePressed ();


protected:
    // virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);


private:
    QString _fileName;
    QPixmap _correctedImage;
    QGraphicsTextItem *_info;
    ClickableItemText *_geoInfo;
    QGraphicsItemGroup *_rating;
    ExifMetadata _pictureData;

    QGeoServiceProvider *_geoProvider;
    QGeoCodingManager  *_geoManager;
    QGeoCodeReply *_reverseGeocodeReply;

    QPixmap correctOrientationPicture(QPixmap src);
    QPixmap scaledImage(QPixmap src);

    void loadPicture ();
    QGraphicsTextItem *createInfo();
    ClickableItemText *createGeoInfo ();
    QGraphicsItemGroup *createRating ();

    void setRatingVisible (bool visible);

    void refreshRating ();

    void centerOnScene ();
    void setChildrenPos ();

    AnimatedItemPicture *createStar (bool, int left, int top);
};

#endif // ANIMATEDITEMPICTURE_H
