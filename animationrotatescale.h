#ifndef ANIMATIONROTATESCALE_H
#define ANIMATIONROTATESCALE_H

#include "pictureanimation.h"

class AnimationRotateScale : public AbstractPictureAnimation
{
public:
    QAbstractAnimation *getAnimationIn (AnimatedItem *target, int duration, int parentWidth);
    QAbstractAnimation *getAnimationOut (AnimatedItem *target, int duration, int parentWidth);
};

#endif // ANIMATIONROTATESCALE_H
