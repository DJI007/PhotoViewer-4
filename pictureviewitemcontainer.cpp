#include "pictureviewitemcontainer.h"

#include <QFileInfo>
#include <QGeoCoordinate>
#include <QGeoAddress>
#include <QPropertyAnimation>
#include <QGraphicsScene>

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
    _rotating = false;

    _rating = NULL;
    _info = NULL;
    _geoInfo = NULL;

    setBrush(Qt::NoBrush);
    setPen(Qt::NoPen);

    if (fileName.endsWith("mp4") || fileName.toLower().endsWith("mts") || fileName.toLower().endsWith("avi")) {
        //  _item = new VideoItem(fileName, this);
        _item = new VideoItemPhonon(fileName, this);
    }
    else {
        _item = new ObjectPixmapItem(fileName, this);
    }

    dynamic_cast<QGraphicsItem *> (_item)->setParentItem(this);

    connect (dynamic_cast<QObject *> (_item),
             SIGNAL(itemLoaded()),
             this,
             SLOT(setItemLoaded()));

    _geoManager = NULL;
    _geoProvider = new QGeoServiceProvider("osm");
    if (_geoProvider) {
        if (_geoProvider->error() == _geoProvider->NoError) {
            _geoManager = _geoProvider->geocodingManager();
        }
        else {
            qDebug () << _geoProvider->errorString();
        }
    }
}

PictureViewItemContainer::~PictureViewItemContainer()
{
    delete _geoProvider;
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
    this->setRect(0, 0, this->scene()->width(), this->scene()->height());
    _item->load ();

    graphicsItem()->setAcceptHoverEvents(true);
}

void PictureViewItemContainer::setShowTime(int time)
{
    _item->setShowTime (time);
    if (time > 0) {
        disconnect(dynamic_cast<QObject *> (_item), SIGNAL(showTimeEnded()), 0, 0);
        connect(dynamic_cast<QObject *> (_item), SIGNAL(showTimeEnded()),
                this, SLOT(setShowTimeEnded()));
    }
}

void PictureViewItemContainer::setShowTimeEnded()
{
    emit showTimeEnded ();
}

void PictureViewItemContainer::setItemLoaded()
{
    _info = createInfo();
    _geoInfo = createGeoInfo();
    _rating = createRating();

    _info->hide();
    _geoInfo->hide();
    _rating->hide();

    _info->setParentItem (this);
    _geoInfo->setParentItem(this);
    _rating->setParentItem(this);

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
    QGraphicsItemGroup *result;

    result = new QGraphicsItemGroup ();

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

    rect = this->boundingRect();

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
    this->setRect(0, 0, this->scene()->width(), this->scene()->height());
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
    if (_info && show != _isInfoVisible) {
        AbstractPictureAnimation *anim;
        QAnimationGroup *group;

        if (show) {
            _info->show();
            _geoInfo->show();
            _rating->show();
        }

        //setInfoRatingPosition();

        group = new QParallelAnimationGroup();
        anim = new AnimationScale();

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

        if (show) {
            group->addAnimation(anim->getAnimationIn(_info, 500, _info->boundingRect().width()));
        }
        else {
            group->addAnimation(anim->getAnimationOut(_info, 500, _info->boundingRect().width()));
        }

        if (show) {
            group->addAnimation(anim->getAnimationIn(_geoInfo, 500, _info->boundingRect().width()));
        }
        else {
            group->addAnimation(anim->getAnimationOut(_geoInfo, 500, _info->boundingRect().width()));
        }

        group->start(QAbstractAnimation::DeleteWhenStopped);

        delete anim;

        _isInfoVisible = show;
    }
}

void PictureViewItemContainer::setRating(int value)
{
    _item->metadata()->setRating(value);

    delete _rating;

    _rating = createRating();
    _rating->setParentItem(this);

    //setInfoVisible(true);
    setInfoRatingPosition ();
}

double PictureViewItemContainer::latitude()
{
    return _item->metadata()->gpsLatitude();
}

double PictureViewItemContainer::longitude()
{
    return _item->metadata()->gpsLongitude();
}

void PictureViewItemContainer::beginItemAnimationIn()
{
    showInfo(false);
    _item->beginItemAnimationIn();
}

void PictureViewItemContainer::endItemAnimationIn()
{
    if (_infoVisible) {
        showInfo(true);
    }

    _item->endItemAnimationIn();
}

void PictureViewItemContainer::beginItemAnimationOut()
{
    showInfo(false);
    _item->beginItemAnimationOut();
}

void PictureViewItemContainer::endItemAnimationOut()
{
    _item->endItemAnimationOut();
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

        doRotation (-90);
    }
}

void PictureViewItemContainer::rotatePictureRight()
{
    if (_item->rotateRight ()) {
        if (_infoVisible) {
            showInfo(false);
        }

        doRotation (90);
    }
}

void PictureViewItemContainer::doRotation(int angle)
{
    QPropertyAnimation *animRotate;
    QPropertyAnimation *animScale;
    QAnimationGroup *anim;
    int transformX;
    int transformY;
    QGraphicsItem *gTarget;
    qreal scaleFactor;
    qreal width;
    qreal height;
    qreal sceneWidth;
    qreal sceneHeight;

    if (_rotating) {
        return;
    }

    _rotating = true;

    this->disconnect(SIGNAL(beginRotateAnimation()));
    this->disconnect(SIGNAL(endRotateAnimation()));

    connect (this, SIGNAL(beginRotateAnimation()),
             dynamic_cast<QObject *> (_item), SLOT(beginRotateAnimation()));
    connect (this, SIGNAL(endRotateAnimation()),
             dynamic_cast<QObject *> (_item), SLOT(endRotateAnimation()));

    gTarget = dynamic_cast<QGraphicsItem *> (_item);

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

    gTarget->setTransformOriginPoint((qreal) transformX, (qreal) transformY);

    anim = new QParallelAnimationGroup();

    animRotate = new QPropertyAnimation(dynamic_cast<QObject *> (_item), "itemRotation");
    animRotate->setDuration(1200);
    animRotate->setStartValue(_item->itemRotation());
    animRotate->setEndValue(_item->itemRotation() + angle);
    animRotate->setEasingCurve(QEasingCurve::InExpo);

    animScale = new QPropertyAnimation(dynamic_cast<QObject *> (_item), "itemScale");
    animScale->setDuration(1200);
    animScale->setStartValue(_item->itemScale());
    animScale->setEndValue(scaleFactor);
    animScale->setEasingCurve(QEasingCurve::InExpo);

    anim->addAnimation(animRotate);
    anim->addAnimation(animScale);

    connect(anim,
            SIGNAL(finished()),
            this,
            SLOT(on_endRotateAnimation ()));

    emit beginRotateAnimation();

    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void PictureViewItemContainer::on_endRotateAnimation ()
{
    emit endRotateAnimation();

    _rotating = false;

    _item->refresh();

    setInfoRatingPosition();

    if (_infoVisible) {
        showInfo(true);
    }
}

qreal PictureViewItemContainer::itemRotation()
{
    return _item->itemRotation();
}

qreal PictureViewItemContainer::itemScale()
{
    return _item->itemScale();
}

void PictureViewItemContainer::setItemRotation(qreal angle)
{
    _item->setItemRotation(angle);
}

void PictureViewItemContainer::setItemScale(qreal scale)
{
    _item->setItemScale(scale);
}
