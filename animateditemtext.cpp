#include "animateditemtext.h"

#include <QGraphicsSceneMouseEvent>
#include <QDebug>

void AnimatedItemText::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit (leftMousePressed());
    }
}
