#include "animationfade.h"

#include <QPropertyAnimation>

#include "pictureanimation.h"

QAbstractAnimation *AnimationFade::getAnimationIn(AnimatedItemPicture *target, int duration, int parentWidth)
{
    QPropertyAnimation *anim;

    Q_UNUSED(parentWidth);

    target->setOpacity(0);

    anim = new QPropertyAnimation(target, "opacity");
    anim->setDuration(duration);
    anim->setStartValue(1);
    anim->setEndValue(0);
    anim->setEasingCurve(QEasingCurve::OutExpo);

    return anim;

}

QAbstractAnimation *AnimationFade::getAnimationOut(AnimatedItemPicture *target, int duration, int parentWidth)
{
    QPropertyAnimation *anim;

    Q_UNUSED(parentWidth);

    anim = new QPropertyAnimation(target, "opacity");
    anim->setDuration(duration);
    anim->setStartValue(0);
    anim->setEndValue(1);
    anim->setEasingCurve(QEasingCurve::OutExpo);

    return anim;
}
