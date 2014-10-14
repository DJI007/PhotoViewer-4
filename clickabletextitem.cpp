#include "clickabletextitem.h"

#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QApplication>

ClickableTextItem::ClickableTextItem (QGraphicsItem *parent)
    : AnimatedTextItem(parent)
{

}

void ClickableTextItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (_isClickable) {
        if (event->button() == Qt::LeftButton) {
            emit (leftMousePressed());
        }
    }
}

void ClickableTextItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);

    if (_isClickable)
        QApplication::setOverrideCursor(QCursor(Qt::PointingHandCursor));
}

void ClickableTextItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);

    if (_isClickable)
        QApplication::restoreOverrideCursor();
}

bool ClickableTextItem::isClickable()
{
    return _isClickable;
}

void ClickableTextItem::setIsClickable(bool value)
{
    _isClickable = value;
    setAcceptHoverEvents(value);
}
