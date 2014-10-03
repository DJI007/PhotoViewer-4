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
    virtual void on_beginItemAnimation () = 0;
    virtual void on_endItemAnimation () = 0;

private:

};

#endif // ANIMATEDITEM_H
