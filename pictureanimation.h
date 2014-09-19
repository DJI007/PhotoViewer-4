#ifndef PICTUREANIMATION_H
#define PICTUREANIMATION_H

#include <QAbstractAnimation>

#include "animateditempicture.h"

class AbstractPictureAnimation
{
public:
    virtual QAbstractAnimation *getAnimationIn (AnimatedItemPicture *target, int duration, int parentWidth) = 0;
    virtual QAbstractAnimation *getAnimationOut (AnimatedItemPicture *target, int duration, int parentWidth) = 0;
};



#endif // PICTUREANIMATION_H
