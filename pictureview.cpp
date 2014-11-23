#include "pictureview.h"

#include <QMouseEvent>
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>
#include <QTimeLine>
#include <QGraphicsItemAnimation>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>

#include "animationfade.h"
#include "animationrotate.h"
#include "animationrotatemove.h"
#include "animationrotatescale.h"
#include "animationrotatefade.h"
#include "animationscale.h"
#include "animationslide.h"


PictureView::PictureView(QWidget *parent) :
    QGraphicsView(parent)
{
    _pictureScene = new QGraphicsScene(this);
    _pictureScene->setObjectName("gsScene");

    _currentItem = NULL;
    _currentAnimation = NULL;
    _prevItem = NULL;

    this->setScene(_pictureScene);
    this->setNormalBackground();

    _animations.append(new AnimationFade());
    _animations.append(new AnimationRotate());
    _animations.append(new AnimationRotateMove());
    _animations.append(new AnimationRotateFade());
    _animations.append(new AnimationRotateScale());
    _animations.append(new AnimationScale());
    _animations.append(new AnimationSlide(AnimationSlide::SlideDirection::LeftToRight));
    _animations.append(new AnimationSlide(AnimationSlide::SlideDirection::RightToLeft));

    _infoVisible = true;
    _showTime = 0;
}

PictureView::~PictureView ()
{
    delete _pictureScene;

    while (!_animations.isEmpty()) {
        delete _animations.takeFirst();
    }
}

void PictureView::mouseDoubleClickEvent (QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit mouseDoubleClick(event);
    }
}

void PictureView::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);

    emit mouseMove(event);
}

bool PictureView::hasPicture()
{
    return _pictureScene->items().count() > 0;
}

void PictureView::resize()
{
    qreal x;
    qreal y;

    x = 0;
    y = 0;

    _pictureScene->setSceneRect (x, y, this->width() - 2, this->height() - 2);

    if (_currentItem != NULL) {
        _currentItem->resize();
    }
}

void PictureView::loadPicture(QString fileName)
{
    if (_currentAnimation) {
        _currentAnimation->stop();
        on_finishPrevItemAnimation();
    }

    if (_currentItem != NULL) {
        _currentItem->setShowTime(0);
    }

    _prevItem = _currentItem;
    if (_prevItem) {
        disconnect(this, SIGNAL(beginItemAnimationIn()), _prevItem, 0);
        disconnect(this, SIGNAL(endItemAnimationIn()), _prevItem, 0);
        connect(this, SIGNAL(beginItemAnimationOut()),
                _prevItem, SLOT(beginItemAnimationOut()));
        connect(this, SIGNAL(endItemAnimationOut()),
                _prevItem, SLOT(endItemAnimationOut()));
    }

    _currentItem = new PictureViewItemContainer(fileName, this);
    connect(dynamic_cast<QObject *> (_currentItem), SIGNAL(showTimeEnded()),
            this, SLOT(on_showTimeEnded()));

    connect (dynamic_cast<QObject *> (_currentItem),
             SIGNAL(requestMapWindow(double,double,double)),
             this,
             SLOT(on_itemRequestMapWindow(double,double,double)));
}

void PictureView::showPicture(PictureAnimationType animType)
{
    if (_currentItem != NULL) {
        _currentAnimationType = animType;
        connect (_currentItem,
                 SIGNAL(itemLoaded()),
                 this,
                 SLOT(on_itemLoaded ()));
        connect (this,
                 SIGNAL(beginItemAnimationIn()),
                 _currentItem,
                 SLOT(beginItemAnimationIn()));
        connect (this,
                 SIGNAL(endItemAnimationIn()),
                 _currentItem,
                 SLOT(endItemAnimationIn()));

        // _pictureScene->addItem(_currentItem->graphicsItem());
        _pictureScene->addItem(_currentItem);
        _currentItem->setInfoVisible(_infoVisible);
        _currentItem->load();
    }
}

void PictureView::on_itemLoaded()
{
    if (_currentAnimationType != PictureAnimationType::None) {
        QAbstractAnimation *animIn = NULL;
        QAbstractAnimation *animOut = NULL;
        AnimationSlide slide;

        _currentAnimation = new QParallelAnimationGroup();

        switch (_currentAnimationType) {
        case PictureAnimationType::Random:
            int current;

            current = qrand() % _animations.count();
            // animIn = _animations.at(current)->getAnimationIn(_currentItem->item(), ANIMATION_DURATION_MILLISECONDS, this->width());
            animIn = _animations.at(current)->getAnimationIn(_currentItem, ANIMATION_DURATION_MILLISECONDS, this->width());

            current = qrand() % _animations.count();
            // animOut = _animations.at(current)->getAnimationOut(_prevItem->item(), ANIMATION_DURATION_MILLISECONDS, this->width());
            animOut = _animations.at(current)->getAnimationOut(_prevItem, ANIMATION_DURATION_MILLISECONDS, this->width());
            break;

        case PictureAnimationType::LeftToRight:
            slide.setDirection (AnimationSlide::SlideDirection::LeftToRight);
            // animIn = slide.getAnimationIn(_currentItem->item(), ANIMATION_DURATION_MILLISECONDS, this->width());
            // animOut = slide.getAnimationOut(_prevItem->item(), ANIMATION_DURATION_MILLISECONDS, this->width());
            animIn = slide.getAnimationIn(_currentItem, ANIMATION_DURATION_MILLISECONDS, this->width());
            animOut = slide.getAnimationOut(_prevItem, ANIMATION_DURATION_MILLISECONDS, this->width());
            break;

        case PictureAnimationType::RightToLeft:
            slide.setDirection (AnimationSlide::SlideDirection::RightToLeft);
            // animIn = slide.getAnimationIn(_currentItem->item(), ANIMATION_DURATION_MILLISECONDS, this->width());
            // animOut = slide.getAnimationOut(_prevItem->item(), ANIMATION_DURATION_MILLISECONDS, this->width());
            animIn = slide.getAnimationIn(_currentItem, ANIMATION_DURATION_MILLISECONDS, this->width());
            animOut = slide.getAnimationOut(_prevItem, ANIMATION_DURATION_MILLISECONDS, this->width());
            break;

        case PictureAnimationType::None:  // To supress compile warning
            animIn = NULL;
            animOut = NULL;
            break;
        }

        if (animIn != NULL) {
            connect(animIn,
                    SIGNAL(finished()),
                    this,
                    SLOT(on_finishCurrentItemAnimation ()));

            _currentAnimation->addAnimation(animIn);
        }

        if (animOut != NULL) {
            connect(animOut,
                    SIGNAL(finished()),
                    this,
                    SLOT(on_finishPrevItemAnimation()));

            _currentAnimation->addAnimation(animOut);
        }

        emit beginItemAnimationIn();
        emit beginItemAnimationOut();
        _currentAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else {
        emit endItemAnimationIn();
        delete _prevItem;
        _currentAnimation = NULL;
    }
}

void PictureView::cleanPicture()
{
    if (_currentItem != NULL) {
        delete _currentItem;
        _currentItem = NULL;
    }

    if (_prevItem != NULL) {
        delete _prevItem;
        _prevItem = NULL;
    }
}

void PictureView::on_finishPrevItemAnimation()
{
    emit endItemAnimationOut();
    delete _prevItem;
}

void PictureView::on_finishCurrentItemAnimation()
{
    _currentItem->setShowTime(_showTime);

    emit endItemAnimationIn();
    _currentAnimation = NULL;
}

void PictureView::on_itemRequestMapWindow (double latitude, double longitude, double altitude)
{
    emit requestMapWindow(latitude, longitude, altitude);
}

void PictureView::setPictureRating(int rating)
{
    _currentItem->setRating (rating);
}

void PictureView::setNormalBackground()
{
    QBrush brush;

    brush.setStyle(Qt::BrushStyle::Dense4Pattern);
    brush.setColor(Qt::GlobalColor::lightGray);

    this->setBackgroundBrush(brush);
}

void PictureView::setFullScreenBackground()
{
    QBrush brush;

    brush.setStyle(Qt::BrushStyle::SolidPattern);
    brush.setColor(Qt::GlobalColor::black);

    this->setBackgroundBrush(brush);
}

double PictureView::pictureLatitude()
{
    return _currentItem->latitude();
}

double PictureView::pictureLongitude()
{
    return _currentItem->longitude();
}

void PictureView::setInfoVisible(bool visible)
{
    if ((!visible) || (visible && !_infoVisible)) {
        _currentItem->setInfoVisible(visible);
    }

    _infoVisible = visible;
}

void PictureView::setShowTime(int time)
{
    _showTime = time;
    _currentItem->setShowTime (time);
}

void PictureView::on_showTimeEnded()
{
    emit showTimeEnded ();
}

void PictureView::rotatePictureLeft()
{
    if (_currentItem) {
        _currentItem->rotatePictureLeft();
    }
}

void PictureView::rotatePictureRight()
{
    if (_currentItem) {
        _currentItem->rotatePictureRight();
    }
}
