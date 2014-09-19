#ifndef ANIMATIONFADE_H
#define ANIMATIONFADE_H

#include "pictureanimation.h"

class AnimationFade : public AbstractPictureAnimation
{
public:
    QAbstractAnimation *getAnimationIn (AnimatedItemPicture *target, int duration, int parentWidth);
    QAbstractAnimation *getAnimationOut (AnimatedItemPicture *target, int duration, int parentWidth);
};

#endif // ANIMATIONFADE_H
