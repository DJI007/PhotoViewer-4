#ifndef ANIMATEDITEMVIDEO_H
#define ANIMATEDITEMVIDEO_H

#include <QGraphicsVideoItem>
#include <QGraphicsTextItem>
#include <QMediaPlayer>

#include "animateditem.h"

class AnimatedItemVideo : public AnimatedItem, public QGraphicsVideoItem
{
public:
    explicit AnimatedItemVideo(QString fileName, QObject *parent = 0);
    ~AnimatedItemVideo();

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

    void endAnimation();

signals:

public slots:

private:
    QMediaPlayer *_player;
    QString _fileName;

    QGraphicsTextItem *_info;

    QGraphicsTextItem *createInfo();
    void setChildrenPos ();

};

#endif // ANIMATEDITEMVIDEO_H
