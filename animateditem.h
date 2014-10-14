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
    virtual void on_beginItemAnimationIn () {}
    virtual void on_endItemAnimationIn ()  {}
    virtual void on_beginItemAnimationOut () {}
    virtual void on_endItemAnimationOut ()  {}


private:

};

#endif // ANIMATEDITEM_H
