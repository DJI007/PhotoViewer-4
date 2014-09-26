#ifndef ANIMATIONFADE_H
#define ANIMATIONFADE_H

#include <QGraphicsItem>
#include "pictureanimation.h"

class AnimationFade : public AbstractPictureAnimation
{
public:
    QAbstractAnimation *getAnimationIn (AnimatedItem *target, int duration, int parentWidth);
    QAbstractAnimation *getAnimationOut (AnimatedItem *target, int duration, int parentWidth);
};

#endif // ANIMATIONFADE_H
