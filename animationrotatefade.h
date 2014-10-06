#ifndef ANIMATIONROTATEFADE_H
#define ANIMATIONROTATEFADE_H

#include "pictureanimation.h"


class AnimationRotateFade : public AbstractPictureAnimation
{
public:
    QAbstractAnimation *getAnimationIn (AnimatedItem *target, int duration, int parentWidth);
    QAbstractAnimation *getAnimationOut (AnimatedItem *target, int duration, int parentWidth);
};

#endif // ANIMATIONROTATEFADE_H
