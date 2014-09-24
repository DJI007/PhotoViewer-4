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
#include "animationscale.h"
#include "animationslide.h"


PictureView::PictureView(QWidget *parent) :
    QGraphicsView(parent)
{
    _pictureScene = new QGraphicsScene(this);
    _pictureScene->setObjectName("gsScene");

    _currentPicture = NULL;
    _currentAnimation = NULL;
    _prevPicture = NULL;

    this->setScene(_pictureScene);
    this->setNormalBackground();

    _animations.append(new AnimationFade());
    _animations.append(new AnimationRotate());
    _animations.append(new AnimationRotateMove());
    _animations.append(new AnimationScale());
    _animations.append(new AnimationSlide(AnimationSlide::SlideDirection::LeftToRight));
    _animations.append(new AnimationSlide(AnimationSlide::SlideDirection::RightToLeft));

}

PictureView::~PictureView ()
{
    delete _pictureScene;
}

void PictureView::mouseDoubleClickEvent (QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit mouseDoubleClick();
    }
}

void PictureView::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);
    emit mouseMove();
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

    if (_currentPicture != NULL) {
        _currentPicture->resize();
    }
}

void PictureView::loadPicture(QString fileName)
{
    if (_currentAnimation) {
        _currentAnimation->stop();
        on_finishPrevPictureAnimation();
    }

    _prevPicture = _currentPicture;
    _currentPicture = new AnimatedItemPicture (fileName, this);
    connect (_currentPicture,
             SIGNAL(requestMapWindow(double,double,double)),
             this,
             SLOT(on_pictureRequestMapWindow(double,double,double)));
}

void PictureView::showPicture(PictureAnimationType animType)
{
    if (_currentPicture != NULL) {
        _pictureScene->addItem(_currentPicture);
        _currentPicture->load();
    }

    if (animType != PictureAnimationType::None) {
        QAbstractAnimation *animIn = NULL;
        QAbstractAnimation *animOut = NULL;
        AnimationSlide slide;

        _currentAnimation = new QParallelAnimationGroup();

        switch (animType) {
        case PictureAnimationType::Random:
            int current;

            current = qrand() % _animations.count();
            animIn = _animations.at(current)->getAnimationIn(_prevPicture, ANIMATION_DURATION_MILLISECONDS, this->width());

            current = qrand() % _animations.count();
            animOut = _animations.at(current)->getAnimationOut(_currentPicture, ANIMATION_DURATION_MILLISECONDS, this->width());
            break;

        case PictureAnimationType::LeftToRight:
            slide.setDirection (AnimationSlide::SlideDirection::LeftToRight);
            animIn = slide.getAnimationIn(_prevPicture, ANIMATION_DURATION_MILLISECONDS, this->width());
            animOut = slide.getAnimationOut(_currentPicture, ANIMATION_DURATION_MILLISECONDS, this->width());
            break;

        case PictureAnimationType::RightToLeft:
            slide.setDirection (AnimationSlide::SlideDirection::RightToLeft);
            animIn = slide.getAnimationIn(_prevPicture, ANIMATION_DURATION_MILLISECONDS, this->width());
            animOut = slide.getAnimationOut(_currentPicture, ANIMATION_DURATION_MILLISECONDS, this->width());
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
                    SLOT(on_finishPrevPictureAnimation ()));


            _currentAnimation->addAnimation(animIn);
            _currentAnimation->addAnimation(animOut);

            _currentAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        }
    }
    else {
        delete _prevPicture;
        _currentAnimation = NULL;
    }
}

void PictureView::cleanPicture()
{
    if (_currentPicture != NULL) {
        delete _currentPicture;
        _currentPicture = NULL;
    }

    if (_prevPicture != NULL) {
        delete _prevPicture;
        _prevPicture = NULL;
    }
}

void PictureView::on_finishPrevPictureAnimation()
{
    delete _prevPicture;
    _currentAnimation = NULL;
}

void PictureView::on_pictureRequestMapWindow (double latitude, double longitude, double altitude)
{
    emit requestMapWindow(latitude, longitude, altitude);
}


void PictureView::setPictureRating(int rating)
{
    _currentPicture->setPictureRating (rating);
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
    return _currentPicture->pictureLatitude();
}

double PictureView::pictureLongitude()
{
    return _currentPicture->pictureLongitude();
}
