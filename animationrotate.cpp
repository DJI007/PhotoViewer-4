#include "animationrotate.h"

#include <QPropertyAnimation>

#include "pictureanimation.h"

QAbstractAnimation *AnimationRotate::getAnimationIn(AnimatedItemPicture *target, int duration, int parentWidth)
{
    QPropertyAnimation *anim;

    Q_UNUSED(parentWidth);

    target->setTransformOriginPoint(0, 0);

    anim = new QPropertyAnimation(target, "rotation");
    anim->setDuration(duration);
    anim->setStartValue(0);
    anim->setEndValue(-180);
    anim->setEasingCurve(QEasingCurve::OutExpo);

    return anim;
}

QAbstractAnimation *AnimationRotate::getAnimationOut(AnimatedItemPicture *target, int duration, int parentWidth)
{
    QPropertyAnimation *anim;

    Q_UNUSED(parentWidth);

    target->setTransformOriginPoint(0, 0);

    anim = new QPropertyAnimation(target, "rotation");
    anim->setDuration(duration);
    anim->setStartValue(-180);
    anim->setEndValue(0);
    anim->setEasingCurve(QEasingCurve::OutExpo);

    return anim;
}
