#ifndef ANIMATIONROTATEMOVE_H
#define ANIMATIONROTATEMOVE_H

#include "pictureanimation.h"

class AnimationRotateMove : public AbstractPictureAnimation
{
public:
    QAbstractAnimation *getAnimationIn (AnimatedItemPicture *target, int duration, int parentWidth);
    QAbstractAnimation *getAnimationOut (AnimatedItemPicture *target, int duration, int parentWidth);
};

#endif // ANIMATIONROTATEMOVE_H
