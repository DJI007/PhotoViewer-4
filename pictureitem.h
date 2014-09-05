#ifndef PICTUREITEM_H
#define PICTUREITEM_H

#include <QGraphicsPixmapItem>

class PictureItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

    // Change picture position property for QPropertyAnimation
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:
    PictureItem(const QPixmap& pixmap, QObject* parent = 0);
    ~PictureItem();

    // Animate position of this class
    void animatePosition(const QPointF& start, const QPointF& end);

signals:

public slots:
    void animationFinished();


};

#endif // PICTUREITEM_H
