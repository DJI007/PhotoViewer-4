#ifndef ANIMATIONSLIDE_H
#define ANIMATIONSLIDE_H

#include <QGraphicsItem>
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

    QAbstractAnimation *getAnimationIn (AnimatedItem *target, int duration, int parentWidth);
    QAbstractAnimation *getAnimationOut (AnimatedItem *target, int duration, int parentWidth);

    void setDirection (SlideDirection direction);

private:
    SlideDirection _direction;
};

#endif // ANIMATIONSLIDE_H
