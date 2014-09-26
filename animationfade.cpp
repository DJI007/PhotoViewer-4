#include "animationfade.h"

#include <QPropertyAnimation>

QAbstractAnimation *AnimationFade::getAnimationIn(AnimatedItem *target, int duration, int parentWidth)
{
    QPropertyAnimation *anim;

    Q_UNUSED(parentWidth);

    (dynamic_cast<QGraphicsItem *> (target))->setOpacity(0);

    anim = new QPropertyAnimation(dynamic_cast<QObject *> (target), "opacity");
    anim->setDuration(duration);
    anim->setStartValue(0);
    anim->setEndValue(1);
    anim->setEasingCurve(QEasingCurve::OutExpo);

    return anim;

}

QAbstractAnimation *AnimationFade::getAnimationOut(AnimatedItem *target, int duration, int parentWidth)
{
    QPropertyAnimation *anim;

    Q_UNUSED(parentWidth);

    anim = new QPropertyAnimation(dynamic_cast<QObject *> (target), "opacity");
    anim->setDuration(duration);
    anim->setStartValue(1);
    anim->setEndValue(0);
    anim->setEasingCurve(QEasingCurve::OutExpo);

    return anim;
}
