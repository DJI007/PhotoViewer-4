#include "animateditemtext.h"

#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QApplication>

void AnimatedItemText::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (_isClickable) {
        if (event->button() == Qt::LeftButton) {
            emit (leftMousePressed());
        }
    }
}

void AnimatedItemText::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);

    if (_isClickable)
        QApplication::setOverrideCursor(QCursor(Qt::PointingHandCursor));
}

void AnimatedItemText::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);

    if (_isClickable)
        QApplication::restoreOverrideCursor();
}

bool AnimatedItemText::isClickable()
{
    return _isClickable;
}

void AnimatedItemText::setIsClickable(bool value)
{
    _isClickable = value;
    setAcceptHoverEvents(value);
}
