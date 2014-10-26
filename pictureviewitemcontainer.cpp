#include "pictureviewitemcontainer.h"

#include <QFileInfo>
#include <QGeoCoordinate>
#include <QGeoAddress>
#include <QPropertyAnimation>
#include <QGraphicsScene>

#include "videoitem.h"
#include "videoitemphonon.h"
#include "objectpixmapitem.h"

#include <QAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>

#include "pictureanimation.h"
#include "animationfade.h"
#include "animationrotate.h"
#include "animationslide.h"
#include "animationrotatemove.h"
#include "animationrotatefade.h"
#include "animationrotatescale.h"
#include "animationscale.h"

#include <QDebug>


PictureViewItemContainer::PictureViewItemContainer(QString fileName, QObject *parent) :
    QObject(parent)
{
    _fileName = fileName;
    _infoVisible = true;
    _isInfoVisible = false;

    _rating = NULL;
    _info = NULL;
    _geoInfo = NULL;

    if (fileName.endsWith("mp4") || fileName.toLower().endsWith("mts") || fileName.toLower().endsWith("avi")) {
        //  _item = new VideoItem(fileName, this);
        _item = new VideoItemPhonon(fileName, this);
    }
    else {
        _item = new ObjectPixmapItem(fileName, this);
    }

    connect (dynamic_cast<QObject *> (_item),
             SIGNAL(itemLoaded()),
             this,
             SLOT(on_itemLoaded()));

    _geoProvider = new QGeoServiceProvider("osm");
    if (_geoProvider) {
        if (_geoProvider->error() != _geoProvider->NoError) {
            qDebug () << _geoProvider->errorString();
        }

        _geoManager = _geoProvider->geocodingManager();
    }
}

PictureViewItemContainer::~PictureViewItemContainer()
{
}

PictureViewItem *PictureViewItemContainer::item()
{
    return _item;
}

QGraphicsItem *PictureViewItemContainer::graphicsItem()
{
    return dynamic_cast<QGraphicsItem *> (_item);
}

void PictureViewItemContainer::load()
{
/*
    _info = createInfo();
    _geoInfo = createGeoInfo();
    _rating = createRating();
*/
    _item->load ();

    graphicsItem()->setAcceptHoverEvents(true);
}

void PictureViewItemContainer::setShowTime(int time)
{
    _item->setShowTime (time);
    if (time > 0) {
        connect(dynamic_cast<QObject *> (_item), SIGNAL(showTimeEnded()),
                this, SLOT(on_showTimeEnded()));
    }
}

void PictureViewItemContainer::on_showTimeEnded()
{
    emit showTimeEnded ();
}

void PictureViewItemContainer::on_itemLoaded()
{
    _info = createInfo();
    _geoInfo = createGeoInfo();
    _rating = createRating();

    _info->setParentItem (this->graphicsItem());
    _geoInfo->setParentItem(this->graphicsItem());
    _rating->setParentItem(this->graphicsItem());

    setInfoRatingPosition();

    emit itemLoaded ();
}

AnimatedTextItem *PictureViewItemContainer::createInfo()
{
    AnimatedTextItem *item;
    QString msg;
    QFileInfo info;

    info.setFile(_fileName);

    msg = "<span style=\"background-color: black; color: white; margin:5px 5px 5px 5px\">";
    msg += info.fileName();
    msg += "<br />";
    msg += _item->getDate().toString(Qt::SystemLocaleLongDate);
    msg += "</span>";

    item = new AnimatedTextItem();
    item->setHtml(msg);

    return item;
}

ClickableTextItem *PictureViewItemContainer::createGeoInfo()
{
    ClickableTextItem *item;
    QString msg;

    item = new ClickableTextItem(this->graphicsItem());

    if (_item->metadata()->hasGpsInfo()) {
        QGeoCoordinate coord;

        coord.setLatitude(_item->metadata()->gpsLatitude());
        coord.setLongitude(_item->metadata()->gpsLongitude());
        coord.setAltitude(_item->metadata()->gpsAltitude());

        if (_geoManager) {
            _reverseGeocodeReply = _geoManager->reverseGeocode(coord);

            connect (_reverseGeocodeReply,
                     SIGNAL(error(QGeoCodeReply::Error,QString)),
                     this,
                     SLOT(on_reverseGeocode_error(QGeoCodeReply::Error,QString)));
            connect (_reverseGeocodeReply,
                     SIGNAL(finished()),
                     this,
                     SLOT(on_reverseGeocode_finished()));
        }

        msg = "<span style=\"background-color: black; color: white; margin:5px 5px 5px 5px\">";
        msg += QString::number(_item->metadata()->gpsLatitude()) + " " + _item->metadata()->gpsLatitudeRef() + " ";
        msg += QString::number(_item->metadata()->gpsLongitude()) + " " + _item->metadata()->gpsLongitudeRef();
        msg += "</span>";

        item->setIsClickable(true);
        item->setHtml(msg);

        connect (item, SIGNAL(leftMousePressed()), this, SLOT(on_geoInfo_leftMousePressed()));
    }

    return item;
}

QGraphicsItemGroup *PictureViewItemContainer::createRating()
{
    int rating;
    int left;
    QRectF rect;
    QGraphicsItemGroup *result;

    result = new QGraphicsItemGroup ();

    rect = this->graphicsItem()->boundingRect();

    rating = _item->metadata()->rating();
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

ObjectPixmapItem *PictureViewItemContainer::createStar (bool isOn, int left, int top)
{
    ObjectPixmapItem *item;
    QPixmap *star;

    if (isOn) {
        star = new QPixmap(":/images/images/star-on.png");
    }
    else {
        star = new QPixmap(":/images/images/star-off.png");
    }

    item = new ObjectPixmapItem(star->scaledToHeight(20, Qt::SmoothTransformation), this);
    item->setPos(left, top);

    delete star;

    return item;
}

void PictureViewItemContainer::setInfoRatingPosition ()
{
    QRectF rect;

    rect = this->graphicsItem()->boundingRect();

    if (_info) {
        _info->setPos(rect.left(), rect.bottom() - 40);
    }

    if (_geoInfo) {
        _geoInfo->setPos(rect.left(), rect.top() + 5);
    }

    if (_rating) {
        qreal left;
        qreal top;

        left = rect.right() - (10 + (20 * 5));   // margin: 10, star width: 20
        top = rect.top() + 5;

        _rating->setPos(left, top);
    }
}


void PictureViewItemContainer::resize()
{
    _item->resize();
    setInfoRatingPosition();
}

void PictureViewItemContainer::setInfoVisible(bool visible)
{
    _infoVisible = visible;
    showInfo (visible);
}

void PictureViewItemContainer::showInfo(bool show)
{
    if (show != _isInfoVisible) {
        AbstractPictureAnimation *anim;
        QAnimationGroup *group;

        //setInfoRatingPosition();

        group = new QParallelAnimationGroup();
        anim = new AnimationScale();

        if (_rating) {
            for (int i = 0; i < _rating->childItems().count(); i++) {
                ObjectPixmapItem *current;

                current = dynamic_cast<ObjectPixmapItem *> (_rating->childItems()[i]);

                if (show) {
                    group->addAnimation(anim->getAnimationIn(current, 500, _rating->boundingRect().width()));
                }
                else {
                    group->addAnimation(anim->getAnimationOut(current, 500, _rating->boundingRect().width()));
                }
            }
        }

        if (_info) {
            if (show) {
                group->addAnimation(anim->getAnimationIn(_info, 500, _info->boundingRect().width()));
            }
            else {
                group->addAnimation(anim->getAnimationOut(_info, 500, _info->boundingRect().width()));
            }
        }

        if (_geoInfo) {
            if (show) {
                group->addAnimation(anim->getAnimationIn(_geoInfo, 500, _info->boundingRect().width()));
            }
            else {
                group->addAnimation(anim->getAnimationOut(_geoInfo, 500, _info->boundingRect().width()));
            }
        }

        group->start(QAbstractAnimation::DeleteWhenStopped);

        _isInfoVisible = show;
    }
}

void PictureViewItemContainer::setRating(int value)
{
    _item->metadata()->setRating(value);

    delete _rating;

    _rating = createRating();
    _rating->setParentItem(this->graphicsItem());

    //setInfoVisible(true);
    setInfoRatingPosition ();
}

double PictureViewItemContainer::latitude()
{
    return 0;
}

double PictureViewItemContainer::longitude()
{
    return 0;
}

void PictureViewItemContainer::on_beginItemAnimationIn()
{
    _item->on_beginItemAnimationIn();
}

void PictureViewItemContainer::on_endItemAnimationIn()
{
    _item->on_endItemAnimationIn();
/*
    if (_infoVisible) {
        showInfo(true);
    }
*/
}

void PictureViewItemContainer::on_beginItemAnimationOut()
{
    _item->on_beginItemAnimationOut();
}

void PictureViewItemContainer::on_endItemAnimationOut()
{
    _item->on_endItemAnimationOut();
}

void PictureViewItemContainer::on_reverseGeocode_error(QGeoCodeReply::Error error, const QString &errorString)
{
    Q_UNUSED(error);

    qDebug () << "PictureViewItemContainer::on_reverseGeocode_error: " << errorString;

    _reverseGeocodeReply->deleteLater();
}

void PictureViewItemContainer::on_reverseGeocode_finished()
{
    if (_reverseGeocodeReply->locations().count() > 0) {
        QGeoLocation loc;

        loc = _reverseGeocodeReply->locations()[0];

        QString msg;

        msg = "<span style=\"background-color: black; color: white; margin:5px 5px 5px 5px\">";
        msg += loc.address().text().replace(",", "<br />");
        msg += "</span>";

        _geoInfo->setHtml(msg);
    }

    _reverseGeocodeReply->deleteLater();
}

void PictureViewItemContainer::on_geoInfo_leftMousePressed()
{
    emit requestMapWindow(_item->metadata()->gpsLatitude(),
                          _item->metadata()->gpsLongitude(),
                          _item->metadata()->gpsAltitude());
}

void PictureViewItemContainer::rotatePictureLeft()
{
    if (_item->rotateLeft ()) {
        if (_infoVisible) {
            showInfo(false);
        }

        doRotation (true);
    }
}

void PictureViewItemContainer::rotatePictureRight()
{
    if (_item->rotateRight ()) {
        if (_infoVisible) {
            showInfo(false);
        }

        doRotation (false);
    }
}

void PictureViewItemContainer::doRotation(bool left)
{
    QPropertyAnimation *animRotate;
    QPropertyAnimation *animScale;
    QAnimationGroup *anim;
    int transformX;
    int transformY;
    int endValue;
    QGraphicsItem *gTarget;
    qreal scaleFactor;
    qreal width;
    qreal height;
    qreal sceneWidth;
    qreal sceneHeight;

    gTarget = dynamic_cast<QGraphicsItem *> (_item);

    if (left) {
        endValue = -90;
    }
    else {
        endValue = 90;
    }

    sceneWidth = gTarget->scene()->width();
    sceneHeight = gTarget->scene()->height();
    width = gTarget->boundingRect().width();
    height = gTarget->boundingRect().height();

    scaleFactor = sceneHeight / width;
    if ((height * scaleFactor) > sceneWidth) {
        scaleFactor = sceneWidth / height;
    }

    transformX = width / 2;
    transformY = height / 2;

    gTarget->setTransformOriginPoint(transformX, transformY);

    anim = new QParallelAnimationGroup();
    // anim = new QSequentialAnimationGroup();

    animRotate = new QPropertyAnimation(dynamic_cast<QObject *> (_item), "rotation");
    animRotate->setDuration(1200);
    animRotate->setStartValue(0);
    animRotate->setEndValue(endValue);
    animRotate->setEasingCurve(QEasingCurve::InExpo);

    animScale = new QPropertyAnimation(dynamic_cast<QObject *> (_item), "scale");
    animScale->setDuration(1200);
    animScale->setStartValue((qreal) 1);
    animScale->setEndValue(scaleFactor);
    animScale->setEasingCurve(QEasingCurve::InExpo);

    anim->addAnimation(animRotate);
    anim->addAnimation(animScale);

    connect(anim,
            SIGNAL(finished()),
            this,
            SLOT(on_finishRotateAnimation ()));

    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void PictureViewItemContainer::on_finishRotateAnimation ()
{
    QGraphicsItem *gItem;

    gItem = dynamic_cast<QGraphicsItem *> (_item);
    gItem->setRotation(0);
    gItem->setScale(1);

    _item->refresh();

    setInfoRatingPosition();

    if (_infoVisible) {
        showInfo(true);
    }
}
