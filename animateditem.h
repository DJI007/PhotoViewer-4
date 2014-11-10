#ifndef ANIMATEDITEM_H
#define ANIMATEDITEM_H

#include <QObject>
#include <QGraphicsItem>

class AnimatedItem
{
    operator QGraphicsItem* () { return dynamic_cast<QGraphicsItem *> (this); }
    operator QObject* () { return dynamic_cast<QObject *> (this); }

public:

signals:

public slots:
    virtual void beginItemAnimationIn () {}
    virtual void endItemAnimationIn ()  {}
    virtual void beginItemAnimationOut () {}
    virtual void endItemAnimationOut ()  {}

private:

};

#endif // ANIMATEDITEM_H
