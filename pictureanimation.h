#ifndef PICTUREANIMATION_H
#define PICTUREANIMATION_H

#include <QAbstractAnimation>

#include "animateditem.h"

class AbstractPictureAnimation
{
public:
    virtual QAbstractAnimation *getAnimationIn (AnimatedItem *target, int duration, int parentWidth) = 0;
    virtual QAbstractAnimation *getAnimationOut (AnimatedItem *target, int duration, int parentWidth) = 0;
};


#endif // PICTUREANIMATION_H
