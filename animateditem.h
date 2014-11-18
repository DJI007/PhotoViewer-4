#ifndef ANIMATEDITEM_H
#define ANIMATEDITEM_H

#include <QObject>
#include <QGraphicsItem>

class AnimatedItem
{
    operator QGraphicsItem* () { return dynamic_cast<QGraphicsItem *> (this); }
    operator QObject* () { return dynamic_cast<QObject *> (this); }

public:
    virtual qreal itemRotation () = 0;
    virtual qreal itemScale () = 0;

    virtual void setItemRotation (qreal angle) = 0;
    virtual void setItemScale (qreal scale) = 0;


signals:

public slots:
    virtual void beginItemAnimationIn () {}
    virtual void endItemAnimationIn ()  {}
    virtual void beginItemAnimationOut () {}
    virtual void endItemAnimationOut ()  {}

private:

};

#endif // ANIMATEDITEM_H
