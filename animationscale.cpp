#include "animationscale.h"

#include <QPropertyAnimation>

#include "pictureanimation.h"

QAbstractAnimation *AnimationScale::getAnimationIn(AnimatedItemPicture *target, int duration, int parentWidth)
{
    QPropertyAnimation *anim;
    qreal transformX;
    qreal transformY;

    Q_UNUSED(parentWidth);

    transformX = target->boundingRect().width() / 2;
    transformY = target->boundingRect().height() / 2;

    target->setTransformOriginPoint(transformX, transformY);

    anim = new QPropertyAnimation(target, "scale");
    anim->setDuration(duration);
    anim->setStartValue(1);
    anim->setEndValue(0.1);
    anim->setEasingCurve(QEasingCurve::OutExpo);

    return anim;
}

QAbstractAnimation *AnimationScale::getAnimationOut(AnimatedItemPicture *target, int duration, int parentWidth)
{
    QPropertyAnimation *anim;
    qreal transformX;
    qreal transformY;

    Q_UNUSED(parentWidth);

    transformX = target->boundingRect().width() / 2;
    transformY = target->boundingRect().height() / 2;

    target->setTransformOriginPoint(transformX, transformY);

    anim = new QPropertyAnimation(target, "scale");
    anim->setDuration(duration);
    anim->setStartValue(0.1);
    anim->setEndValue(1);
    anim->setEasingCurve(QEasingCurve::OutExpo);

    return anim;
}
