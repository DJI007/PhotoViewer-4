#ifndef ANIMATIONROTATE_H
#define ANIMATIONROTATE_H

#include "pictureanimation.h"

class AnimationRotate : public AbstractPictureAnimation
{
public:
    QAbstractAnimation *getAnimationIn (AnimatedItemPicture *target, int duration, int parentWidth);
    QAbstractAnimation *getAnimationOut (AnimatedItemPicture *target, int duration, int parentWidth);
};

#endif // ANIMATIONROTATE_H
