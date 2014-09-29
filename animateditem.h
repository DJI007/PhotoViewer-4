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
    explicit AnimatedItem (QString fileName, QObject *parent = 0);
    ~AnimatedItem ();

    void load ();
    void resize ();
    void setRating(int rating);
    double latitude ();
    double longitude ();

    void setInfoVisible (bool visible);

    QPointF animationPos ();
    qreal animationOpacity ();
    qreal animationRotation ();
    qreal animationScale ();

    void setAnimationPos (QPointF);
    void setAnimationOpacity (qreal);
    void setAnimationRotation (qreal);
    void setAnimationScale (qreal);

    void endAnimation ();

    QGraphicsItem *item ();

signals:
    void requestMapWindow (double latitude, double longitude, double altitude);

public slots:

private:
    QGraphicsItem *_item;

};

#endif // ANIMATEDITEM_H
