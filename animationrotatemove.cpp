#include "animationrotatemove.h"

#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

QAbstractAnimation *AnimationRotateMove::getAnimationIn(AnimatedItem *target, int duration, int parentWidth)
{
    QPropertyAnimation *anim;
    QParallelAnimationGroup *rotateMove;
    qreal transformX;
    qreal transformY;
    QGraphicsItem *gTarget;

    gTarget = dynamic_cast<QGraphicsItem *> (target);

    transformX = gTarget->boundingRect().width() / 2;
    transformY = gTarget->boundingRect().height() / 2;

    gTarget->setTransformOriginPoint(transformX, transformY);

    rotateMove = new QParallelAnimationGroup();

    anim = new QPropertyAnimation(dynamic_cast<QObject *> (target), "pos");
    anim->setDuration(duration);
    anim->setStartValue(QPointF(parentWidth, 0));
    //anim->setEndValue(QPointF(target->animationPos().x(), target->animationPos().y()));
    anim->setEndValue(QPointF(0, 0));
    anim->setEasingCurve(QEasingCurve::OutExpo);

    rotateMove->addAnimation(anim);

    anim = new QPropertyAnimation(dynamic_cast<QObject *> (target), "rotation");
    anim->setDuration(duration);
    anim->setStartValue(180);
    anim->setEndValue(0);
    anim->setEasingCurve(QEasingCurve::OutExpo);

    rotateMove->addAnimation(anim);

    return rotateMove;
}

QAbstractAnimation *AnimationRotateMove::getAnimationOut(AnimatedItem *target, int duration, int parentWidth)
{
    QPropertyAnimation *anim;
    QParallelAnimationGroup *rotateMove;
    qreal transformX;
    qreal transformY;
    QGraphicsItem *gTarget;

    gTarget = dynamic_cast<QGraphicsItem *> (target);

    transformX = gTarget->boundingRect().width() / 2;
    transformY = gTarget->boundingRect().height() / 2;

    gTarget->setTransformOriginPoint(transformX, transformY);

    rotateMove = new QParallelAnimationGroup();

    anim = new QPropertyAnimation(dynamic_cast<QObject *> (target), "pos");
    anim->setDuration(duration);
    anim->setStartValue(QPointF(0, 0));
    //anim->setStartValue(QPointF(QPointF(target->animationPos().x(), target->animationPos().y())));
    anim->setEndValue(QPointF(-parentWidth, 0));
    anim->setEasingCurve(QEasingCurve::OutExpo);

    rotateMove->addAnimation(anim);

    anim = new QPropertyAnimation(dynamic_cast<QObject *> (target), "rotation");
    anim->setDuration(duration);
    anim->setStartValue(0);
    anim->setEndValue(-180);
    anim->setEasingCurve(QEasingCurve::OutExpo);

    rotateMove->addAnimation(anim);

    return rotateMove;
}

