#ifndef ANIMATIONROTATEMOVE_H
#define ANIMATIONROTATEMOVE_H

#include <QGraphicsItem>
#include "pictureanimation.h"

class AnimationRotateMove : public AbstractPictureAnimation
{
public:
    QAbstractAnimation *getAnimationIn (AnimatedItem *target, int duration, int parentWidth);
    QAbstractAnimation *getAnimationOut (AnimatedItem *target, int duration, int parentWidth);
};

#endif // ANIMATIONROTATEMOVE_H
