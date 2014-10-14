#ifndef CLICKABLETEXTITEM_H
#define CLICKABLETEXTITEM_H

#include <QGraphicsTextItem>

#include "animatedtextitem.h"

class ClickableTextItem : public AnimatedTextItem
{
    Q_OBJECT

    Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:
    explicit ClickableTextItem (QGraphicsItem *parent = 0);
    bool isClickable ();
    void setIsClickable (bool value);

signals:
    void leftMousePressed();

public slots:

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

private:
    bool _isClickable = false;

};

#endif // CLICKABLETEXTITEM_H
