#include "pictureview.h"

#include <QMouseEvent>
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>
#include <QTimeLine>
#include <QGraphicsItemAnimation>

PictureView::PictureView(QWidget *parent) :
    QGraphicsView(parent)
{
    _pictureScene = new QGraphicsScene(this);
    _pictureScene->setObjectName("gsScene");

    this->setScene(_pictureScene);

    _picture = NULL;
}

PictureView::~PictureView ()
{
    if (_picture != NULL)
        delete _picture;

    delete _pictureScene;
}

void PictureView::mouseDoubleClickEvent (QMouseEvent *event)
{
   if (event->button() == Qt::LeftButton) {
       emit mouseDoubleClick(event);
   }
}

void PictureView::setPicture(QString fileName)
{
    if (_picture != NULL) {
        delete _picture;
    }

    // qDebug () << "setPicture: " << fileName;

    _fileName = fileName;
    _picture = new QImage (fileName);
    _pictureData.loadData(fileName);
}


bool PictureView::hasPicture()
{
    return (_picture != NULL) && (!_picture->isNull());
}

void PictureView::showPicture()
{
    if (_pictureScene->items().size() > 0) {
        hidePicture ();
    }

    _pictureScene->clear();

    addPicture ();
    addInfo ();
    addRating ();
}

void PictureView::hidePicture()
{
/*
    QGraphicsPixmapItem item;

    item = (QGraphicsPixmapItem *) _pictureScene->items() [0];

    QTimeLine *timer = new QTimeLine(5000);

    timer->setFrameRange(0, 100);


    QGraphicsItemAnimation *animation = new QGraphicsItemAnimation;

    animation->setItem(item);
    animation->setTimeLine(timer);

    for (int i = 0; i < 200; ++i)
        animation->setPosAt(i / 200.0, QPointF(i, i));

    timer->start();
*/
}

void PictureView::setPictureRating(int rating)
{
    _pictureData.setRating(rating);
    addRating ();
}

void PictureView::addPicture()
{
    QGraphicsItem *item;
    QPixmap image;

    image = correctOrientationPicture();
    image = scaledImage (image);

    item = new QGraphicsPixmapItem(image);

    QRect rect;

    rect.adjust(_pictureScene->sceneRect().left(),
                _pictureScene->sceneRect().top(),
                item->boundingRect().width() - 2,
                item->boundingRect().height() - 2);

    _pictureScene->setSceneRect(rect);

    _pictureScene->addItem(item);
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
QPixmap PictureView::correctOrientationPicture()
{
    QPixmap image;
    QTransform trans;

    image = QPixmap::fromImage(*_picture);

    // Set the correct orientation
    switch (_pictureData.orientation())
    {
    case 1:
        // Nothing to do
        break;

    case 2:
        // Horizontal mirror
        image = QPixmap::fromImage(_picture->mirrored(true, false));
        break;

    case 3:
        // Rotate 180 degrees
        trans.rotate(180);
        break;

    case 4:
        // Vertical mirror
        image = QPixmap::fromImage(_picture->mirrored(false, true));
        break;

    case 5:
        // Vertical mirror and rotate 90 degrees
        image = QPixmap::fromImage(_picture->mirrored(false, true));
        trans.rotate(90);
        break;

    case 6:
        // Rotate 90 degrees
        trans.rotate(90);
        break;

    case 7:
        // Horizontal mirror and rotate 90 degrees
        image = QPixmap::fromImage(_picture->mirrored(true, false));
        trans.rotate(90);
        break;

    case 8:
        // Rotate 270 degrees
        trans.rotate(270);
        break;
    }

    return image.transformed(trans);
}

QPixmap PictureView::scaledImage (QPixmap src)
{
    QPixmap image;

    image = src.scaledToWidth(this->width(), Qt::SmoothTransformation);
    if (image.height() > this->height()) {
        image = src.scaledToHeight(this->height(), Qt::SmoothTransformation);
    }

    return image;
}

void PictureView::addInfo()
{
    QGraphicsTextItem *item;
    QString msg;

    msg = "<span style=\"background-color: black; color: white; margin:5px 5px 5px 5px\">";
    msg += _fileName;
    msg += "<br />";
    msg += _pictureData.pictureDate().toString(Qt::SystemLocaleLongDate);
    msg += "</span>";

    QRectF rect;

    rect = _pictureScene->sceneRect();

    item = new QGraphicsTextItem();
    item->setPos(rect.left(), rect.bottom() - 40);
    item->setHtml(msg);

    _pictureScene->addItem(item);
}

void PictureView::addRating()
{
    int rating;
    int left;
    int top;
    QRectF rect;

    rect = _pictureScene->sceneRect();
    rating = _pictureData.rating();
    left = rect.right() - (10 + (20 * 5));   // margin: 10, star width: 20
    top = rect.top() + 5;

    for (int i = 0; i < rating; i++) {
        addStar (true, left, top);
        left += 20;
    }

    for (int i = rating; i < 5; i++) {
        addStar (false, left, top);
        left += 20;
    }
}

void PictureView::addStar (bool isOn, int left, int top)
{
    QGraphicsItem *item;
    QPixmap *star;

    if (isOn) {
        star = new QPixmap(":/images/images/star-on.png");
    }
    else {
        star = new QPixmap(":/images/images/star-off.png");
    }

    item = new QGraphicsPixmapItem(star->scaledToHeight(20, Qt::SmoothTransformation));
    item->setPos(left, top);

    _pictureScene->addItem(item);

    delete star;
}

