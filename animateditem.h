#ifndef ANIMATEDITEM_H
#define ANIMATEDITEM_H

#include <QObject>
#include <QPointF>

class AnimatedItem : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QPointF pos READ animationPos WRITE setAnimationPos)
    Q_PROPERTY(qreal opacity READ animationOpacity WRITE setAnimationOpacity)
    Q_PROPERTY(qreal rotation READ animationRotation WRITE setAnimationRotation)
    Q_PROPERTY(qreal scale READ animationScale WRITE setAnimationScale)

public:
    explicit AnimatedItem (QObject *parent = 0);

    virtual void load () = 0;
    virtual void resize () = 0;
    virtual void setRating(int rating) = 0;
    virtual double latitude () = 0;
    virtual double longitude () = 0;

    virtual void setInfoVisible (bool visible) = 0;

    virtual QPointF animationPos () = 0;
    virtual qreal animationOpacity () = 0;
    virtual qreal animationRotation () = 0;
    virtual qreal animationScale () = 0;

    virtual void setAnimationPos (QPointF) = 0;
    virtual void setAnimationOpacity (qreal) = 0;
    virtual void setAnimationRotation (qreal) = 0;
    virtual void setAnimationScale (qreal) = 0;

    virtual void endAnimation ();

signals:
    void requestMapWindow (double latitude, double longitude, double altitude);

public slots:

};

#endif // ANIMATEDITEM_H
