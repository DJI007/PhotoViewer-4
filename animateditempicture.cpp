#include "animateditempicture.h"

#include <QPropertyAnimation>

AnimatedItemPicture::AnimatedItemPicture(const QPixmap& pixmap, QObject* parent) :
    QObject(parent), QGraphicsPixmapItem(pixmap)
{
    setCacheMode(DeviceCoordinateCache);
}


