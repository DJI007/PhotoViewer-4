#ifndef OBJECTPIXMAPITEM_H
#define OBJECTPIXMAPITEM_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QDateTime>

#include "pictureviewitem.h"
#include "exifmetadata.h"

class ObjectPixmapItem : public QObject,
        public QGraphicsPixmapItem,
        public PictureViewItem
{
    Q_OBJECT

    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
    Q_PROPERTY(qreal itemRotation READ itemRotation WRITE setItemRotation)
    Q_PROPERTY(qreal itemScale READ itemScale WRITE setItemScale)

public:
    explicit ObjectPixmapItem(QString fileName, QObject *parent = 0);
    explicit ObjectPixmapItem(const QPixmap& pixmap, QObject *parent = 0);
    ~ObjectPixmapItem();

    void load ();
    void refresh ();
    void resize ();
    QDateTime getDate ();

    AbstractMetadata *metadata ();

    void setShowTime (int time);

    bool rotateLeft();
    bool rotateRight();

    void setItemRotation (qreal angle);
    void setItemScale (qreal scale);
    qreal itemRotation ();
    qreal itemScale ();

    qreal zoom ();
    void setZoom (qreal zoomPercent);

signals:
    void itemLoaded ();
    void showTimeEnded ();

    void zoomChanged (qreal zoom);

public slots:
    void on_showTimeEnded ();

    void beginRotateAnimation () {}
    void endRotateAnimation ();

protected:
    void connectNotify ( const char * signal );

private:
    QString _fileName;
    ExifMetadata *_pictureData;
    QPixmap _correctedImage;
    QPixmap _realImage;

    QTimer *_showTimer;

    qreal _scale;
    //qreal _minimalZoom;

    void correctOrientationPicture();
    QPixmap scaledImage(QPixmap src);
    void centerOnScene ();

    void calculateScale ();
};

#endif // OBJECTPIXMAPITEM_H
