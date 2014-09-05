#ifndef ANIMATEDITEMPICTURE_H
#define ANIMATEDITEMPICTURE_H

#include <QGraphicsPixmapItem>

class AnimatedItemPicture: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

    // Change picture position property for QPropertyAnimation
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:
    explicit AnimatedItemPicture(const QPixmap& pixmap, QObject* parent = 0);

signals:

public slots:

};

#endif // ANIMATEDITEMPICTURE_H
