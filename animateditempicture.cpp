#include "animateditempicture.h"

#include <QPropertyAnimation>
#include <QGraphicsScene>
#include <QDebug>
#include <QGraphicsWidget>

AnimatedItemPicture::AnimatedItemPicture(const QPixmap& pixmap, QObject* parent) :
    QObject(parent), QGraphicsPixmapItem(pixmap)
{
    setCacheMode(DeviceCoordinateCache);
}

AnimatedItemPicture::AnimatedItemPicture(const QString fileName, QObject* parent) :
    QObject(parent)
{
    setCacheMode(DeviceCoordinateCache);

    _fileName = fileName;
    _pictureData.loadData(fileName);

    _info = NULL;
    _rating = NULL;
}

AnimatedItemPicture::~AnimatedItemPicture()
{
    if (_info != NULL)
        delete _info;

    if (_rating != NULL)
        delete _rating;
}

void AnimatedItemPicture::load ()
{
    loadPicture ();

    _info = createInfo();
    _rating = createRating();

    _info->setParentItem (this);
    _rating->setParentItem(this);

    this->centerOnScene ();
    this->setChildrenPos();
}

void AnimatedItemPicture::setPictureRating(int rating)
{
    _pictureData.setRating(rating);
    refreshRating ();
}

void AnimatedItemPicture::resize()
{
    this->setPixmap(scaledImage(_correctImage));
    this->centerOnScene ();
    this->setChildrenPos();
}

void AnimatedItemPicture::loadPicture()
{
    QPixmap image;

    image.load (_fileName);
    if (!image.isNull()) {
        _correctImage = correctOrientationPicture(image);
        image = scaledImage (_correctImage);
        this->setPixmap (image);
    }
}

QString AnimatedItemPicture::toString()
{
    return _fileName;
}

/**
 * @brief Get the current picture in the correct orientation
 * @return
 *
 * Rotate the selected picture in the correct orientation.
 * EXIF Orientation can has this values:
 * Value    0th Row        0th Column
 *   1        top          left side
 *   2        top          right side
 *   3        bottom       right side
 *   4        bottom       left side
 *   5        left side    top
 *   6        right side   top
 *   7        right side   bottom
 *   8        left side    bottom
 *
 * Here is what the letter F would look like if it were tagged correctly and
 * displayed by a program that ignores the orientation tag:
 *    1        2       3      4          5           6          7            8
 *  888888  888888      88  88      8888888888  88                  88  8888888888
 *  88          88      88  88      88  88      88  88          88  88      88  88
 *  8888      8888    8888  8888    88          8888888888  8888888888          88
 *  88          88      88  88
 *  88          88  888888  888888
 *
 * http://sylvana.net/jpegcrop/exif_orientation.html
 */
QPixmap AnimatedItemPicture::correctOrientationPicture(QPixmap image)
{
    QTransform trans;

    // Set the correct orientation
    switch (_pictureData.orientation())
    {
    case 1:
        // Nothing to do
        break;

    case 2:
        // Horizontal mirror
        trans.scale(-1, 1);
        break;

    case 3:
        // Rotate 180 degrees
        trans.rotate(180);
        break;

    case 4:
        // Vertical mirror
        trans.scale(1, -1);
        break;

    case 5:
        // Vertical mirror and rotate 90 degrees
        trans.scale(1, -1);
        trans.rotate(90);
        break;

    case 6:
        // Rotate 90 degrees
        trans.rotate(90);
        break;

    case 7:
        // Horizontal mirror and rotate 90 degrees
        trans.scale(-1, 1);
        trans.rotate(90);
        break;

    case 8:
        // Rotate 270 degrees
        trans.rotate(270);
        break;
    }

    return image.transformed(trans);
}

QPixmap AnimatedItemPicture::scaledImage (QPixmap src)
{
    QPixmap image;

    image = src.scaledToWidth(this->scene()->width(), Qt::SmoothTransformation);
    if (image.height() > this->scene()->height()) {
        image = src.scaledToHeight(this->scene()->height(), Qt::SmoothTransformation);
    }

    return image;
}

void AnimatedItemPicture::centerOnScene()
{
    qreal newX;
    qreal newY;
    qreal adjustWidth;
    qreal adjustHeight;
    QPixmap image;

    adjustWidth = this->scene()->width();
    adjustHeight = this->scene()->height();
    image = this->pixmap();

    if (image.width() < adjustWidth) {
        newX = (adjustWidth / 2) - (image.width() / 2);
    }
    else {
        newX = 0;
    }

    if (image.height() < adjustHeight) {
        newY = (adjustHeight / 2) - (image.height() / 2);
    }
    else {
        newY = 0;
    }

    QTransform transform;

    transform.translate(newX, newY);
    this->setTransform(transform);
}

void AnimatedItemPicture::setChildrenPos ()
{
    // QRectF rect(0, 0, this->scene()->width(), this->scene()->height());
    QRectF rect;

    //rect = this->boundingRect();
    rect = this->pixmap().rect();

    _info->setPos(rect.left(), rect.bottom() - 40);

    qreal left;
    qreal top;

    left = rect.right() - (10 + (20 * 5));   // margin: 10, star width: 20
    top = rect.top() + 5;
    _rating->setPos(left, top);

    // qDebug() << "AnimatedItemPicture::resize: " << rect.width() << "-.-" << rect.height();
    // qDebug() << "AnimatedItemPicture::resize: scene: " << this->scene()->width() << "-.-" << this->scene()->height();
}

AnimatedItemText *AnimatedItemPicture::createInfo()
{
    AnimatedItemText *item;
    QString msg;

    msg = "<span style=\"background-color: black; color: white; margin:5px 5px 5px 5px\">";
    msg += _fileName;
    msg += "<br />";
    msg += _pictureData.pictureDate().toString(Qt::SystemLocaleLongDate);
    msg += "</span>";

    QRectF rect;

    rect = this->boundingRect();

    item = new AnimatedItemText();
    item->setHtml(msg);
    // item->setPos(rect.left(), rect.bottom() - 40);

    return item;
}

QGraphicsItemGroup *AnimatedItemPicture::createRating()
{
    int rating;
    int left;
    QRectF rect;
    QGraphicsItemGroup *result;

    result = new QGraphicsItemGroup ();

    rect = this->boundingRect();

    rating = _pictureData.rating();
    // left = rect.right() - (10 + (20 * 5));   // margin: 10, star width: 20
    // top = rect.top() + 5;
    left = 0;

    for (int i = 0; i < rating; i++) {
        result->addToGroup(createStar (true, left, 0));
        left += 20;
    }

    for (int i = rating; i < 5; i++) {
        result->addToGroup(createStar (false, left, 0));
        left += 20;
    }

    return result;
}

void AnimatedItemPicture::refreshRating()
{
    delete _rating;

    _rating = createRating ();
    _rating->setParentItem(this);
    this->setChildrenPos();
}

AnimatedItemPicture *AnimatedItemPicture::createStar (bool isOn, int left, int top)
{
    AnimatedItemPicture *item;
    QPixmap *star;

    if (isOn) {
        star = new QPixmap(":/images/images/star-on.png");
    }
    else {
        star = new QPixmap(":/images/images/star-off.png");
    }

    item = new AnimatedItemPicture(star->scaledToHeight(20, Qt::SmoothTransformation));
    item->setPos(left, top);

    // _pictureScene->addItem(item);

    delete star;

    return item;
}
