#ifndef ANIMATEDTEXTITEM_H
#define ANIMATEDTEXTITEM_H

#include <QGraphicsTextItem>

#include "animateditem.h"

class AnimatedTextItem : public QGraphicsTextItem,
        public AnimatedItem
{
    Q_OBJECT

    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation)
    Q_PROPERTY(qreal scale READ scale WRITE setScale)

    Q_PROPERTY(qreal itemRotation READ itemRotation WRITE setItemRotation)
    Q_PROPERTY(qreal itemScale READ itemScale WRITE setItemScale)

public:
    explicit AnimatedTextItem(QGraphicsItem *parent = 0);

    void setItemRotation (qreal angle);
    void setItemScale (qreal scale);
    qreal itemRotation ();
    qreal itemScale ();

signals:

public slots:

};

#endif // ANIMATEDTEXTITEM_H
