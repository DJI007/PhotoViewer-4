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

void PictureView::resize()
{
    qreal x;
    qreal y;

    x = 0;
    y = 0;

    _pictureScene->setSceneRect (x, y, this->width() - 2, this->height() - 2);

     _currentPicture->resize();
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
                         SLOT(on_finishPrevPictureAnimation ()));
        _currentAnimation->addAnimation(anim);

        anim = new QPropertyAnimation(_currentPicture, "pos");
        anim->setDuration(1000);
        anim->setStartValue(QPointF(startX, 0));
        anim->setEndValue(QPointF(endX, 0));
        anim->setEasingCurve(QEasingCurve::OutExpo);
        _currentAnimation->addAnimation(anim);

        _currentAnimation->start(QAbstractAnimation::DeleteWhenStopped);
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

