#ifndef ANIMATIONSCALE_H
#define ANIMATIONSCALE_H

#include <QGraphicsItem>
#include "pictureanimation.h"

class AnimationScale : public AbstractPictureAnimation
{
public:
    QAbstractAnimation *getAnimationIn (AnimatedItem *target, int duration, int parentWidth);
    QAbstractAnimation *getAnimationOut (AnimatedItem *target, int duration, int parentWidth);
};

#endif // ANIMATIONSCALE_H
