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

signals:
    void itemLoaded ();
    void showTimeEnded ();

public slots:
    void on_showTimeEnded ();
    void endRotateLeftAnimation ();
    void endRotateRightAnimation ();

protected:
    void connectNotify ( const char * signal );

private:
    QString _fileName;
    ExifMetadata *_pictureData;
    QPixmap _correctedImage;
    QPixmap _realImage;

    QTimer *_showTimer;

    QPixmap correctOrientationPicture(QPixmap src);
    QPixmap scaledImage(QPixmap src);
    void centerOnScene ();
};

#endif // OBJECTPIXMAPITEM_H
