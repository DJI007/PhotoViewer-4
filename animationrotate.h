#ifndef ANIMATIONROTATE_H
#define ANIMATIONROTATE_H

#include <QGraphicsItem>
#include "pictureanimation.h"

class AnimationRotate : public AbstractPictureAnimation
{
public:
    QAbstractAnimation *getAnimationIn (AnimatedItem *target, int duration, int parentWidth);
    QAbstractAnimation *getAnimationOut (AnimatedItem *target, int duration, int parentWidth);
};

#endif // ANIMATIONROTATE_H
