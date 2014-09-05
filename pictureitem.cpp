#include "pictureitem.h"

#include <QPropertyAnimation>

PictureItem::PictureItem(const QPixmap& pixmap, QObject* parent) :
    QObject(parent), QGraphicsPixmapItem(pixmap)
{
    setCacheMode(DeviceCoordinateCache);
}

PictureItem::~PictureItem()
{
}

void PictureItem::animatePosition(const QPointF& start, const QPointF& end)
{
    // Start animate this class
    QPropertyAnimation* anim = new QPropertyAnimation(this, "pos");

    // 2 second duration animation
    anim->setDuration(2000);
    // position to start animation
    anim->setStartValue(start);
    // end position of animation
    anim->setEndValue(end);
    // easing curve
    anim->setEasingCurve(QEasingCurve::InOutElastic);

    // Listen animation finished signal
    QObject::connect(anim, SIGNAL(finished()), this, SLOT(animationFinished()));

    // Start animation and delete QPropertyAnimation class on the end
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void PictureItem::animationFinished()
{
    // This slot is called when animation ends
}
