#include "animationrotatefade.h"

#include <QGraphicsItem>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

QAbstractAnimation *AnimationRotateFade::getAnimationIn(AnimatedItem *target, int duration, int parentWidth)
{
    QPropertyAnimation *anim;
    QParallelAnimationGroup *rotateFade;
    qreal transformX;
    qreal transformY;
    QGraphicsItem *gTarget;

    Q_UNUSED(parentWidth);

    gTarget = dynamic_cast<QGraphicsItem *> (target);

    gTarget->setOpacity(0);

    transformX = gTarget->boundingRect().width() / 2;
    transformY = gTarget->boundingRect().height() / 2;

    gTarget->setTransformOriginPoint(transformX, transformY);

    rotateFade = new QParallelAnimationGroup();

    anim = new QPropertyAnimation(dynamic_cast<QObject *> (target), "opacity");
    anim->setDuration(duration);
    anim->setStartValue(0);
    anim->setEndValue(1);
    anim->setEasingCurve(QEasingCurve::OutExpo);

    rotateFade->addAnimation(anim);

    anim = new QPropertyAnimation(dynamic_cast<QObject *> (target), "itemRotation");
    anim->setDuration(duration);
    anim->setStartValue(360);
    anim->setEndValue(0);
    anim->setEasingCurve(QEasingCurve::OutExpo);

    rotateFade->addAnimation(anim);

    return rotateFade;
}

QAbstractAnimation *AnimationRotateFade::getAnimationOut(AnimatedItem *target, int duration, int parentWidth)
{
    QPropertyAnimation *anim;
    QParallelAnimationGroup *rotateFade;
    qreal transformX;
    qreal transformY;
    QGraphicsItem *gTarget;

    Q_UNUSED(parentWidth);

    gTarget = dynamic_cast<QGraphicsItem *> (target);

    transformX = gTarget->boundingRect().width() / 2;
    transformY = gTarget->boundingRect().height() / 2;

    gTarget->setTransformOriginPoint(transformX, transformY);

    rotateFade = new QParallelAnimationGroup();

    anim = new QPropertyAnimation(dynamic_cast<QObject *> (target), "opacity");
    anim->setDuration(duration);
    anim->setStartValue(1);
    anim->setEndValue(0);
    anim->setEasingCurve(QEasingCurve::OutExpo);

    rotateFade->addAnimation(anim);

    anim = new QPropertyAnimation(dynamic_cast<QObject *> (target), "itemRotation");
    anim->setDuration(duration);
    anim->setStartValue(0);
    anim->setEndValue(360);
    anim->setEasingCurve(QEasingCurve::OutExpo);

    rotateFade->addAnimation(anim);

    return rotateFade;
}

