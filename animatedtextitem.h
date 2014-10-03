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

public:

signals:

public slots:
    virtual void on_beginItemAnimation () {}
    virtual void on_endItemAnimation () {}

};

#endif // ANIMATEDTEXTITEM_H
