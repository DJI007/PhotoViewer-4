#ifndef CLICKABLEITEMTEXT_H
#define CLICKABLEITEMTEXT_H

#include <QGraphicsTextItem>

class ClickableItemText : public QGraphicsTextItem
{
    Q_OBJECT

    // Change picture position property for QPropertyAnimation
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:
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

#endif // CLICKABLEITEMTEXT_H
