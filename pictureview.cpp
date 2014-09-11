#include "pictureview.h"

#include <QMouseEvent>
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>
#include <QTimeLine>
#include <QGraphicsItemAnimation>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>


PictureView::PictureView(QWidget *parent) :
    QGraphicsView(parent)
{
    _pictureScene = new QGraphicsScene(this);
    _pictureScene->setObjectName("gsScene");

    _currentPicture = NULL;
    _currentAnimation = NULL;

    this->setScene(_pictureScene);
}

PictureView::~PictureView ()
{
    delete _pictureScene;
}

void PictureView::mouseDoubleClickEvent (QMouseEvent *event)
{
   if (event->button() == Qt::LeftButton) {
       emit mouseDoubleClick(event);
   }
}

bool PictureView::hasPicture()
{
    return _pictureScene->items().count() > 0;
}

void PictureView::changeSize()
{
    resize ();
     _currentPicture->resize();

/*
    QRect rect;

    rect.adjust(0,
                0,
                this->width() - 4,
                this->height() - 4);

    qDebug() << "this: " << this->width() << "-.-" << this->height();
    qDebug() << "scene: " << _pictureScene->sceneRect().width() << "-.-" << _pictureScene->sceneRect().height();


    // _pictureScene->setSceneRect(rect);
    _currentPicture->load();
    */
}

void PictureView::resize()
{
    qreal x;
    qreal y;

    x = 0;
    y = 0;

    _pictureScene->setSceneRect (x, y, this->width() - 2, this->height() - 2);
}

void PictureView::loadPicture(QString fileName)
{
    if (_currentAnimation) {
        _currentAnimation->stop();
        on_finish_outAnimation();
    }

    // _pictureScene->clear();
    _prevPicture = _currentPicture;
    _currentPicture = new AnimatedItemPicture (fileName, this);
    // _pictureScene->addItem(_currentPicture);
    // showPicture (PictureAnimationType::None);
}

void PictureView::showPicture(PictureAnimationType animType)
{
    if (_currentPicture != NULL) {
        _pictureScene->addItem(_currentPicture);
        _currentPicture->load();
    }

    if (animType != PictureAnimationType::None) {
        qreal startX;
        qreal endX;

        if (animType == PictureAnimationType::RightToLeft) {
            startX = this->width();
            endX = 0;
        }
        else {
            startX = -this->width();
            endX = 0;
        }

        QPropertyAnimation *anim;

        _currentAnimation = new QParallelAnimationGroup();

        anim = new QPropertyAnimation(_prevPicture, "pos");
        anim->setDuration(1000);
        anim->setStartValue(QPointF(endX, 0));
        anim->setEndValue(QPointF(-startX, 0));
        anim->setEasingCurve(QEasingCurve::OutExpo);
        QObject::connect(anim,
                         SIGNAL(finished()),
                         this,
                         SLOT(on_finish_outAnimation ()));
        _currentAnimation->addAnimation(anim);

        anim = new QPropertyAnimation(_currentPicture, "pos");
        anim->setDuration(1000);
        anim->setStartValue(QPointF(startX, 0));
        anim->setEndValue(QPointF(endX, 0));
        anim->setEasingCurve(QEasingCurve::OutExpo);
        _currentAnimation->addAnimation(anim);

        _currentAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }


/*
    if (_pictureScene->items().count() > 0) {
        QGraphicsItem *first;

        first = _pictureScene->items() [0];

        _pictureScene->removeItem(first->group());
    }

    QGraphicsItemGroup *pictureGroup;

    pictureGroup = new QGraphicsItemGroup ();
    pictureGroup->addToGroup(createPicture());
    pictureGroup->addToGroup(createRating ());
    pictureGroup->addToGroup(createInfo ());

    _pictureScene->addItem(pictureGroup);

    qDebug() << _pictureScene->items().count();
*/
}

void PictureView::showPictureWithAnimation()
{
/*
   _pictureScene->clear();

    addPicture ();
    addInfo ();
    addRating ();
*/
/*
    QAbstractAnimation *animOut = NULL;

    if (_pictureScene->items().count() > 0) {
        QGraphicsItem *first;

        first = _pictureScene->items() [0];

        animOut = createAnimationOut (first->group());

        QObject::connect(animOut,
                         SIGNAL(finished()),
                         this,
                         SLOT(on_finish_outAnimation ()));

        // animOut->start(QAbstractAnimation::DeleteWhenStopped);
        //_pictureScene->removeItem(first->group());
    }

    QAbstractAnimation *animIn;
    QGraphicsItemGroup *pictureGroup;

    pictureGroup = new QGraphicsItemGroup ();
    pictureGroup->addToGroup(createPicture());
    pictureGroup->addToGroup(createRating ());
    pictureGroup->addToGroup(createInfo ());

    animIn = createAnimationIn (pictureGroup);

    QParallelAnimationGroup *anim;

    anim = new QParallelAnimationGroup();

    if (animOut != NULL) {
        anim->addAnimation (animOut);
    }

    anim->addAnimation (animIn);

    anim->start(QAbstractAnimation::DeleteWhenStopped);

    _pictureScene->addItem(pictureGroup);

    qDebug() << "Animation: " << _pictureScene->items().count();
*/
}

void PictureView::on_finish_outAnimation()
{
    _pictureScene->removeItem(_prevPicture);
    delete _prevPicture;
    _currentAnimation = NULL;
/*
    QGraphicsItem *first;

    first = _pictureScene->items() [0];
    _pictureScene->removeItem(first->group());

    qDebug() << "2" << _pictureScene->items().count();
*/
/*
    QAbstractAnimation *animIn;
    QGraphicsItemGroup *pictureGroup;

    pictureGroup = new QGraphicsItemGroup ();
    pictureGroup->addToGroup(createPicture());
    pictureGroup->addToGroup(createRating ());
    pictureGroup->addToGroup(createInfo ());

    animIn = createAnimationIn (pictureGroup);

    QSequentialAnimationGroup *anim;

    anim = new QSequentialAnimationGroup();
    anim->addAnimation (animIn);

    anim->start(QAbstractAnimation::DeleteWhenStopped);

    _pictureScene->addItem(pictureGroup);
*/
}

void PictureView::setPictureRating(int rating)
{
    _currentPicture->setPictureRating (rating);
/*
    _pictureData.setRating(rating);
    createRating ();
*/
}

/*
AnimatedItemPicture *PictureView::createPicture()
{
    AnimatedItemPicture *item;
    QPixmap image;

    image = correctOrientationPicture();
    image = scaledImage (image);

    item = new AnimatedItemPicture(image);

    QRect rect;

    rect.adjust(_pictureScene->sceneRect().left(),
                _pictureScene->sceneRect().top(),
                item->boundingRect().width() - 2,
                item->boundingRect().height() - 2);
    _pictureScene->setSceneRect(rect);

    //_pictureScene->addItem(item);
    //setAnimationIn (item);

    return item;
}

QAbstractAnimation *PictureView::createAnimationIn (QGraphicsItemGroup *group)
{
    AnimatedItemPicture *picture;
    QPropertyAnimation* anim;

    picture = (AnimatedItemPicture *) group->childItems()[0];

    anim = new QPropertyAnimation(picture, "pos");
    anim->setDuration(2000);
    anim->setStartValue(QPointF(this->width(), 0));
    anim->setEndValue(QPointF(0, 0));
    anim->setEasingCurve(QEasingCurve::OutExpo);

    // anim->start(QAbstractAnimation::DeleteWhenStopped);
    return anim;
}

QAbstractAnimation *PictureView::createAnimationOut(QGraphicsItemGroup *group)
{
    AnimatedItemPicture *picture;
    QPropertyAnimation* anim;

    picture = (AnimatedItemPicture *) group->childItems()[0];

    anim = new QPropertyAnimation(picture, "pos");
    anim->setDuration(2000);
    anim->setStartValue(QPointF(0, 0));
    anim->setEndValue(QPointF(-this->width(), 0));
    anim->setEasingCurve(QEasingCurve::OutExpo);

    // anim->start(QAbstractAnimation::DeleteWhenStopped);
    return anim;
}
*/

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
/*
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

AnimatedItemText *PictureView::createInfo()
{
    AnimatedItemText *item;
    QString msg;

    msg = "<span style=\"background-color: black; color: white; margin:5px 5px 5px 5px\">";
    msg += _fileName;
    msg += "<br />";
    msg += _pictureData.pictureDate().toString(Qt::SystemLocaleLongDate);
    msg += "</span>";

    QRectF rect;

    rect = _pictureScene->sceneRect();

    item = new AnimatedItemText();
    item->setPos(rect.left(), rect.bottom() - 40);
    item->setHtml(msg);

    // _pictureScene->addItem(item);
    return item;
}

QGraphicsItemGroup *PictureView::createRating()
{
    int rating;
    int left;
    int top;
    QRectF rect;
    QGraphicsItemGroup *result;

    result = new QGraphicsItemGroup ();

    rect = _pictureScene->sceneRect();
    rating = _pictureData.rating();
    left = rect.right() - (10 + (20 * 5));   // margin: 10, star width: 20
    top = rect.top() + 5;

    for (int i = 0; i < rating; i++) {
        result->addToGroup(createStar (true, left, top));
        left += 20;
    }

    for (int i = rating; i < 5; i++) {
        result->addToGroup(createStar (false, left, top));
        left += 20;
    }

    return result;
}

AnimatedItemPicture *PictureView::createStar (bool isOn, int left, int top)
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

*/
