#ifndef ANIMATEDITEMPICTURE_H
#define ANIMATEDITEMPICTURE_H

#include <QGraphicsPixmapItem>
#include <QString>

#include "exifmetadata.h"
#include "animateditemtext.h"

class AnimatedItemPicture: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

    // Change picture position property for QPropertyAnimation
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:
    explicit AnimatedItemPicture(const QPixmap& pixmap, QObject* parent = 0);
    explicit AnimatedItemPicture(const QString fileName, QObject* parent = 0);
    ~AnimatedItemPicture ();

    QString toString ();

    void load ();
    void resize ();
    void setPictureRating(int rating);

signals:

public slots:

private:
    QString _fileName;
    QPixmap _correctImage;
    AnimatedItemText *_info;
    QGraphicsItemGroup *_rating;
    ExifMetadata _pictureData;

    QPixmap correctOrientationPicture(QPixmap src);
    QPixmap scaledImage(QPixmap src);

    void loadPicture ();
    AnimatedItemText *createInfo ();
    QGraphicsItemGroup *createRating ();

    void refreshRating ();

    void centerOnScene ();
    void setChildrenPos ();

    AnimatedItemPicture *createStar (bool, int left, int top);
};

#endif // ANIMATEDITEMPICTURE_H
