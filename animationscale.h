#ifndef ANIMATIONSCALE_H
#define ANIMATIONSCALE_H

#include "pictureanimation.h"

class AnimationScale : public AbstractPictureAnimation
{
public:
    QAbstractAnimation *getAnimationIn (AnimatedItemPicture *target, int duration, int parentWidth);
    QAbstractAnimation *getAnimationOut (AnimatedItemPicture *target, int duration, int parentWidth);
};

#endif // ANIMATIONSCALE_H
