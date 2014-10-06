#include "animationrotatescale.h"

#include <QGraphicsItem>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

QAbstractAnimation *AnimationRotateScale::getAnimationIn(AnimatedItem *target, int duration, int parentWidth)
{
    QPropertyAnimation *anim;
    QParallelAnimationGroup *rotateScale;
    qreal transformX;
    qreal transformY;
    QGraphicsItem *gTarget;

    gTarget = dynamic_cast<QGraphicsItem *> (target);

    transformX = gTarget->boundingRect().width() / 2;
    transformY = gTarget->boundingRect().height() / 2;

    gTarget->setTransformOriginPoint(transformX, transformY);

    rotateScale = new QParallelAnimationGroup();

    anim = new QPropertyAnimation(dynamic_cast<QObject *> (target), "scale");
    anim->setDuration(duration);
    anim->setStartValue(0);
    anim->setEndValue(1);
    anim->setEasingCurve(QEasingCurve::OutExpo);

    rotateScale->addAnimation(anim);

    anim = new QPropertyAnimation(dynamic_cast<QObject *> (target), "rotation");
    anim->setDuration(duration);
    anim->setStartValue(360);
    anim->setEndValue(0);
    anim->setEasingCurve(QEasingCurve::OutExpo);

    rotateScale->addAnimation(anim);

    return rotateScale;
}

QAbstractAnimation *AnimationRotateScale::getAnimationOut(AnimatedItem *target, int duration, int parentWidth)
{
    QPropertyAnimation *anim;
    QParallelAnimationGroup *rotateScale;
    qreal transformX;
    qreal transformY;
    QGraphicsItem *gTarget;

    gTarget = dynamic_cast<QGraphicsItem *> (target);

    transformX = gTarget->boundingRect().width() / 2;
    transformY = gTarget->boundingRect().height() / 2;

    gTarget->setTransformOriginPoint(transformX, transformY);

    rotateScale = new QParallelAnimationGroup();

    anim = new QPropertyAnimation(dynamic_cast<QObject *> (target), "scale");
    anim->setDuration(duration);
    anim->setStartValue(1);
    anim->setEndValue(0);
    anim->setEasingCurve(QEasingCurve::OutExpo);

    rotateScale->addAnimation(anim);

    anim = new QPropertyAnimation(dynamic_cast<QObject *> (target), "rotation");
    anim->setDuration(duration);
    anim->setStartValue(0);
    anim->setEndValue(360);
    anim->setEasingCurve(QEasingCurve::OutExpo);

    rotateScale->addAnimation(anim);

    return rotateScale;
}

