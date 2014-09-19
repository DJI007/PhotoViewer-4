#include "animationrotatemove.h"

#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

#include "pictureanimation.h"

QAbstractAnimation *AnimationRotateMove::getAnimationIn(AnimatedItemPicture *target, int duration, int parentWidth)
{
    QPropertyAnimation *anim;
    QParallelAnimationGroup *rotateMove;
    qreal transformX;
    qreal transformY;

    transformX = target->boundingRect().width() / 2;
    transformY = target->boundingRect().height() / 2;

    target->setTransformOriginPoint(transformX, transformY);

    rotateMove = new QParallelAnimationGroup();

    anim = new QPropertyAnimation(target, "pos");
    anim->setDuration(duration);
    anim->setStartValue(QPointF(0, 0));
    anim->setEndValue(QPointF(-parentWidth, 0));
    anim->setEasingCurve(QEasingCurve::OutExpo);

    rotateMove->addAnimation(anim);

    anim = new QPropertyAnimation(target, "rotation");
    anim->setDuration(duration);
    anim->setStartValue(0);
    anim->setEndValue(-180);
    anim->setEasingCurve(QEasingCurve::OutExpo);

    rotateMove->addAnimation(anim);

    return rotateMove;
}

QAbstractAnimation *AnimationRotateMove::getAnimationOut(AnimatedItemPicture *target, int duration, int parentWidth)
{
    QPropertyAnimation *anim;
    QParallelAnimationGroup *rotateMove;
    qreal transformX;
    qreal transformY;

    transformX = target->boundingRect().width() / 2;
    transformY = target->boundingRect().height() / 2;

    target->setTransformOriginPoint(transformX, transformY);

    rotateMove = new QParallelAnimationGroup();

    anim = new QPropertyAnimation(target, "pos");
    anim->setDuration(duration);
    anim->setStartValue(QPointF(parentWidth, 0));
    anim->setEndValue(QPointF(0, 0));
    anim->setEasingCurve(QEasingCurve::OutExpo);

    rotateMove->addAnimation(anim);

    anim = new QPropertyAnimation(target, "rotation");
    anim->setDuration(duration);
    anim->setStartValue(180);
    anim->setEndValue(0);
    anim->setEasingCurve(QEasingCurve::OutExpo);

    rotateMove->addAnimation(anim);

    return rotateMove;
}
