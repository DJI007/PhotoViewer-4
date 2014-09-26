#include "clickableitemtext.h"

#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QApplication>

void ClickableItemText::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (_isClickable) {
        if (event->button() == Qt::LeftButton) {
            emit (leftMousePressed());
        }
    }
}

void ClickableItemText::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);

    if (_isClickable)
        QApplication::setOverrideCursor(QCursor(Qt::PointingHandCursor));
}

void ClickableItemText::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);

    if (_isClickable)
        QApplication::restoreOverrideCursor();
}

bool ClickableItemText::isClickable()
{
    return _isClickable;
}

void ClickableItemText::setIsClickable(bool value)
{
    _isClickable = value;
    setAcceptHoverEvents(value);
}
