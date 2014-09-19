#ifndef ANIMATIONSLIDE_H
#define ANIMATIONSLIDE_H

#include "pictureanimation.h"

class AnimationSlide : public AbstractPictureAnimation
{
public:
    enum SlideDirection {
        LeftToRight,
        RightToLeft
    };

    AnimationSlide ();
    AnimationSlide (SlideDirection direction);

    QAbstractAnimation *getAnimationIn (AnimatedItemPicture *target, int duration, int parentWidth);
    QAbstractAnimation *getAnimationOut (AnimatedItemPicture *target, int duration, int parentWidth);

    void setDirection (SlideDirection direction);

private:
    SlideDirection _direction;
};

#endif // ANIMATIONSLIDE_H
