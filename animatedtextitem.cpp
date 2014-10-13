#include "animatedtextitem.h"

#include <QFont>

AnimatedTextItem::AnimatedTextItem(QGraphicsItem *parent)
    : QGraphicsTextItem(parent)
{
    this->setFont(QFont("MS Serif", 12));
}


