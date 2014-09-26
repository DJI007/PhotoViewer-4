#include "animationscale.h"

#include <QPropertyAnimation>

QAbstractAnimation *AnimationScale::getAnimationIn(AnimatedItem *target, int duration, int parentWidth)
{
    QPropertyAnimation *anim;
    qreal transformX;
    qreal transformY;
    QGraphicsItem *gTarget;

    gTarget = dynamic_cast<QGraphicsItem *> (target);

    Q_UNUSED(parentWidth);

    transformX = gTarget->boundingRect().width() / 2;
    transformY = gTarget->boundingRect().height() / 2;

    gTarget->setTransformOriginPoint(transformX, transformY);

    anim = new QPropertyAnimation(dynamic_cast<QObject *> (target), "scale");
    anim->setDuration(duration);
    anim->setStartValue(0);
    anim->setEndValue(1);
    anim->setEasingCurve(QEasingCurve::OutExpo);

    return anim;
}

QAbstractAnimation *AnimationScale::getAnimationOut(AnimatedItem *target, int duration, int parentWidth)
{
    QPropertyAnimation *anim;
    qreal transformX;
    qreal transformY;
    QGraphicsItem *gTarget;

    gTarget = dynamic_cast<QGraphicsItem *> (target);

    Q_UNUSED(parentWidth);

    transformX = gTarget->boundingRect().width() / 2;
    transformY = gTarget->boundingRect().height() / 2;

    gTarget->setTransformOriginPoint(transformX, transformY);

    anim = new QPropertyAnimation(dynamic_cast<QObject *> (target), "scale");
    anim->setDuration(duration);
    anim->setStartValue(1);
    anim->setEndValue(0);
    anim->setEasingCurve(QEasingCurve::OutExpo);

    return anim;
}

