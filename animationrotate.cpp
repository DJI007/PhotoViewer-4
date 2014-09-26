#include "animationrotate.h"

#include <QPropertyAnimation>

#include "pictureanimation.h"

QAbstractAnimation *AnimationRotate::getAnimationIn(AnimatedItem *target, int duration, int parentWidth)
{
    QPropertyAnimation *anim;

    Q_UNUSED(parentWidth);

    (dynamic_cast<QGraphicsItem *> (target))->setTransformOriginPoint(0, 0);

    anim = new QPropertyAnimation(dynamic_cast<QObject *> (target), "rotation");
    anim->setDuration(duration);
    anim->setStartValue(-180);
    anim->setEndValue(0);
    anim->setEasingCurve(QEasingCurve::OutExpo);

    return anim;
}

QAbstractAnimation *AnimationRotate::getAnimationOut(AnimatedItem *target, int duration, int parentWidth)
{
    QPropertyAnimation *anim;

    Q_UNUSED(parentWidth);

    (dynamic_cast<QGraphicsItem *> (target))->setTransformOriginPoint(0, 0);

    anim = new QPropertyAnimation(dynamic_cast<QObject *> (target), "rotation");
    anim->setDuration(duration);
    anim->setStartValue(0);
    anim->setEndValue(-180);
    anim->setEasingCurve(QEasingCurve::OutExpo);

    return anim;
}

