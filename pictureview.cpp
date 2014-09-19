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
       emit mouseDoubleClick(event);
   }
}

void PictureView::mouseMoveEvent(QMouseEvent *event)
{
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
}

void PictureView::showPicture(PictureAnimationType animType)
{
    if (_currentPicture != NULL) {
        _pictureScene->addItem(_currentPicture);
        _currentPicture->load();
    }

    if (animType != PictureAnimationType::None) {
        QAbstractAnimation *animIn;
        QAbstractAnimation *animOut;
        AnimationSlide slide;

        _currentAnimation = new QParallelAnimationGroup();

        switch (animType) {
        case PictureAnimationType::Random:
            int current;

            current = qrand() % _animations.count();
            animIn = _animations.at(current)->getAnimationIn(_prevPicture, 2000, this->width());

            current = qrand() % _animations.count();
            animOut = _animations.at(current)->getAnimationOut(_currentPicture, 2000, this->width());
            break;

        case PictureAnimationType::LeftToRight:
            slide.setDirection (AnimationSlide::SlideDirection::LeftToRight);
            animIn = slide.getAnimationIn(_prevPicture, 2000, this->width());
            animOut = slide.getAnimationOut(_currentPicture, 2000, this->width());
            break;

        case PictureAnimationType::RightToLeft:
            slide.setDirection (AnimationSlide::SlideDirection::RightToLeft);
            animIn = slide.getAnimationIn(_prevPicture, 2000, this->width());
            animOut = slide.getAnimationOut(_currentPicture, 2000, this->width());
            break;
        }

        connect(animIn,
                SIGNAL(finished()),
                this,
                SLOT(on_finishPrevPictureAnimation ()));


        _currentAnimation->addAnimation(animIn);
        _currentAnimation->addAnimation(animOut);

        _currentAnimation->start(QAbstractAnimation::DeleteWhenStopped);
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
