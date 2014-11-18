#include "animatedtextitem.h"

#include <QFont>

AnimatedTextItem::AnimatedTextItem(QGraphicsItem *parent)
    : QGraphicsTextItem(parent)
{
    this->setFont(QFont("MS Serif", 10));
}


void AnimatedTextItem::setItemRotation(qreal angle)
{
    QGraphicsItem::setRotation(angle);
}

void AnimatedTextItem::setItemScale(qreal scale)
{
    QGraphicsItem::setScale(scale);
}

qreal AnimatedTextItem::itemRotation()
{
    return QGraphicsItem::rotation();
}

qreal AnimatedTextItem::itemScale()
{
    return QGraphicsItem::scale();
}
