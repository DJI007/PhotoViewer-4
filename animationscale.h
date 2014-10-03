#ifndef ANIMATIONSCALE_H
#define ANIMATIONSCALE_H

#include <QGraphicsItem>
#include "pictureanimation.h"

class AnimationScale : public AbstractPictureAnimation
{
    Q_OBJECT

public:
    QAbstractAnimation *getAnimationIn (AnimatedItem *target, int duration, int parentWidth);
    QAbstractAnimation *getAnimationOut (AnimatedItem *target, int duration, int parentWidth);

/*
private slots:
    void on_animationOutFinished ();
*/
};

#endif // ANIMATIONSCALE_H
