#include "animationslide.h"
#include <QPropertyAnimation>
#include <QPointF>

AnimationSlide::AnimationSlide ()
    :AnimationSlide(SlideDirection::LeftToRight)
{
}

AnimationSlide::AnimationSlide (SlideDirection direction)
{
    _direction = direction;
}

QAbstractAnimation *AnimationSlide::getAnimationIn(AnimatedItemPicture *target, int duration, int parentWidth)
{
    QPropertyAnimation *anim;
    int endX;

    if (_direction == SlideDirection::RightToLeft) {
        endX = -parentWidth;
    }
    else {
        endX = parentWidth;
    }

    target->setTransformOriginPoint(0, 0);

    anim = new QPropertyAnimation(target, "pos");
    anim->setDuration(duration);
    anim->setStartValue(QPointF(0, 0));
    anim->setEndValue(QPointF(endX, 0));
    anim->setEasingCurve(QEasingCurve::OutExpo);

    return anim;
}

QAbstractAnimation *AnimationSlide::getAnimationOut(AnimatedItemPicture *target, int duration, int parentWidth)
{
    QPropertyAnimation *anim;
    int startX;

    if (_direction == SlideDirection::RightToLeft) {
        startX = parentWidth;
    }
    else {
        startX = -parentWidth;
    }

    target->setTransformOriginPoint(0, 0);

    anim = new QPropertyAnimation(target, "pos");
    anim->setDuration(duration);
    anim->setStartValue(QPointF(startX, 0));
    anim->setEndValue(QPointF(0, 0));
    anim->setEasingCurve(QEasingCurve::OutExpo);

    return anim;
}

void AnimationSlide::setDirection(SlideDirection direction)
{
    _direction = direction;
}

