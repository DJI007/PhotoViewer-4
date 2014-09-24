#ifndef ANIMATEDITEMPICTURE_H
#define ANIMATEDITEMPICTURE_H

#include <QGraphicsPixmapItem>
#include <QString>
#include <QGeoServiceProvider>
#include <QGeoCodingManager>

#include "exifmetadata.h"
#include "animateditemtext.h"

class AnimatedItemPicture: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation)
    Q_PROPERTY(qreal scale READ scale WRITE setScale)

public:
    explicit AnimatedItemPicture(const QPixmap& pixmap, QObject* parent = 0);
    explicit AnimatedItemPicture(const QString fileName, QObject* parent = 0);
    ~AnimatedItemPicture ();

    QString toString ();

    void load ();
    void resize ();
    void setPictureRating(int rating);
    double pictureLatitude ();
    double pictureLongitude ();

signals:
    void requestMapWindow (double latitude, double longitude, double altitude);

public slots:
    void on_reverseGeocode_error(QGeoCodeReply::Error error, const QString &errorString);
    void on_reverseGeocode_finished();
    void on_geoInfo_leftMousePressed ();

protected:
    // virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);


private:
    QString _fileName;
    QPixmap _correctImage;
    AnimatedItemText *_info;
    AnimatedItemText *_geoInfo;
    QGraphicsItemGroup *_rating;
    ExifMetadata _pictureData;

    QGeoServiceProvider *_geoProvider;
    QGeoCodingManager  *_geoManager;
    QGeoCodeReply *_reverseGeocodeReply;

    QPixmap correctOrientationPicture(QPixmap src);
    QPixmap scaledImage(QPixmap src);

    void loadPicture ();
    AnimatedItemText *createInfo ();
    AnimatedItemText *createGeoInfo ();
    QGraphicsItemGroup *createRating ();

    void refreshRating ();

    void centerOnScene ();
    void setChildrenPos ();

    AnimatedItemPicture *createStar (bool, int left, int top);
};

#endif // ANIMATEDITEMPICTURE_H
