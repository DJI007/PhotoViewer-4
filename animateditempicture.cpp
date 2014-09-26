#include "animateditempicture.h"

#include <QPropertyAnimation>
#include <QGraphicsScene>
#include <QDebug>
#include <QGraphicsWidget>
#include <QFileInfo>
#include <QGeoAddress>

#include <QAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>

#include "pictureanimation.h"
#include "animationfade.h"
#include "animationrotate.h"
#include "animationslide.h"
#include "animationrotatemove.h"
#include "animationscale.h"

#include "clickableitemtext.h"
#include "mapview.h"

AnimatedItemPicture::AnimatedItemPicture(const QPixmap& pixmap, QObject* parent) :
    QGraphicsPixmapItem(pixmap),
    AnimatedItem(parent)
{
    setCacheMode(DeviceCoordinateCache);

    _info = NULL;
    _geoInfo = NULL;
    _rating = NULL;
    _geoProvider = NULL;
    _geoManager = NULL;
}

AnimatedItemPicture::AnimatedItemPicture(const QString fileName, QObject* parent) :
    AnimatedItem (parent)
{
    setCacheMode(DeviceCoordinateCache);

    _fileName = fileName;
    _pictureData.loadData(fileName);

    _info = NULL;
    _geoInfo = NULL;
    _rating = NULL;

/*
    QStringList providers;

    providers = QGeoServiceProvider::availableServiceProviders();
    qDebug() << "Providers available:";
    for (int i = 0; i < providers.count(); i++) {
        qDebug () << providers[i];
    }
    qDebug() << "<<<<<<<<<<<<<<<<<<<";
*/
    _geoProvider = new QGeoServiceProvider("osm");
    if (_geoProvider) {
        if (_geoProvider->error() != _geoProvider->NoError) {
            qDebug () << _geoProvider->errorString();
        }

        _geoManager = _geoProvider->geocodingManager();
    }
}

AnimatedItemPicture::~AnimatedItemPicture()
{
    // if (_geoProvider)
    //     delete _geoProvider;

    // if (_geoManager)
    //    delete _geoManager;
/*
    if (_mainView) {
        _mainView->hide();
        delete _mainView;
    }
*/
}

void AnimatedItemPicture::load ()
{
    loadPicture ();

    _info = createInfo();
    _geoInfo = createGeoInfo();
    _rating = createRating();

    _info->setParentItem (this);
    _geoInfo->setParentItem(this);
    _rating->setParentItem(this);

    setAcceptHoverEvents(true);

    centerOnScene ();
    setChildrenPos();
}

void AnimatedItemPicture::setRating(int rating)
{
    _pictureData.setRating(rating);
    setRatingVisible(true);
    refreshRating ();
}

void AnimatedItemPicture::resize()
{
    this->setPixmap(scaledImage(_correctedImage));
    this->centerOnScene ();
    this->setChildrenPos();
}

void AnimatedItemPicture::loadPicture()
{
    QPixmap image;

    image.load (_fileName);
    if (!image.isNull()) {
        _correctedImage = correctOrientationPicture(image);
        image = scaledImage (_correctedImage);
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

    //image = src.scaledToWidth(this->scene()->width(), Qt::FastTransformation);
    image = src.scaledToWidth(this->scene()->width(), Qt::SmoothTransformation);
    if (image.height() > this->scene()->height()) {
        // image = src.scaledToHeight(this->scene()->height(), Qt::FastTransformation);
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
    QRectF rect;

    rect = this->pixmap().rect();

    _info->setPos(rect.left(), rect.bottom() - 40);
    _geoInfo->setPos(rect.left(), rect.top() + 5);

    qreal left;
    qreal top;

    left = rect.right() - (10 + (20 * 5));   // margin: 10, star width: 20
    top = rect.top() + 5;
    _rating->setPos(left, top);
}

QGraphicsTextItem *AnimatedItemPicture::createInfo()
{
    QGraphicsTextItem *item;
    QString msg;
    QString date;
    QFileInfo info;

    info.setFile(_fileName);

    date = _pictureData.pictureDate().toString(Qt::SystemLocaleLongDate);
    if (date == "") {
        date = info.created().toString(Qt::SystemLocaleLongDate);
    }

    msg = "<span style=\"background-color: black; color: white; margin:5px 5px 5px 5px\">";
    msg += info.fileName();
    msg += "<br />";
    msg += date;
    msg += "</span>";

    item = new QGraphicsTextItem();
    item->setHtml(msg);

    return item;
}

ClickableItemText *AnimatedItemPicture::createGeoInfo()
{
    ClickableItemText *item;
    QString msg;

    item = new ClickableItemText();

    if (_pictureData.gpsLatitude() != 0 && _pictureData.gpsLongitude() != 0) {
        QGeoCoordinate coord;

        coord.setLatitude(_pictureData.gpsLatitude());
        coord.setLongitude(_pictureData.gpsLongitude());
        coord.setAltitude(_pictureData.gpsAltitude());

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
        msg += QString::number(_pictureData.gpsLatitude()) + " " + _pictureData.gpsLatitudeRef() + " ";
        msg += QString::number(_pictureData.gpsLongitude()) + " " + _pictureData.gpsLongitudeRef();
        msg += "</span>";

        setAcceptHoverEvents(true);
        item->setIsClickable(true);
        item->setHtml(msg);

        connect (item, SIGNAL(leftMousePressed()), this, SLOT(on_geoInfo_leftMousePressed()));
    }


    return item;
}

void AnimatedItemPicture::on_reverseGeocode_error(QGeoCodeReply::Error error, const QString &errorString)
{
    Q_UNUSED(error);

    qDebug () << "Error on reverse geocode: " << errorString;

    _reverseGeocodeReply->deleteLater();
}

void AnimatedItemPicture::on_reverseGeocode_finished()
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

void AnimatedItemPicture::on_geoInfo_leftMousePressed()
{
    emit requestMapWindow(_pictureData.gpsLatitude(),
                          _pictureData.gpsLongitude(),
                          _pictureData.gpsAltitude());
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

void AnimatedItemPicture::setRatingVisible(bool visible)
{
    AbstractPictureAnimation *anim;
    QAnimationGroup *group;

    group = new QParallelAnimationGroup();
    anim = new AnimationScale();

    for (int i = 0; i < _rating->childItems().count(); i++) {
        AnimatedItemPicture *current;

        current = dynamic_cast<AnimatedItemPicture *> (_rating->childItems()[i]);

        if (visible) {
            group->addAnimation(anim->getAnimationIn(current, 500, _rating->boundingRect().width()));
        }
        else {
            group->addAnimation(anim->getAnimationOut(current, 500, _rating->boundingRect().width()));
        }
    }


    group->start(QAbstractAnimation::DeleteWhenStopped);
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

    delete star;

    return item;
}
/*
void AnimatedItemPicture::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED (event);
}
*/

double AnimatedItemPicture::latitude()
{
    return _pictureData.gpsAltitude();
}

double AnimatedItemPicture::longitude()
{
    return _pictureData.gpsLongitude();
}

void AnimatedItemPicture::setInfoVisible(bool visible)
{
    if (visible) {
        _info->show();
        _geoInfo->show();
        // _rating->show();
    }
    else {
        _info->hide();
        _geoInfo->hide();
        // _rating->hide();
    }

    setRatingVisible(visible);
}

QPointF AnimatedItemPicture::animationPos ()
{
    return pos();
}

qreal AnimatedItemPicture::animationOpacity ()
{
    return opacity();
}

qreal AnimatedItemPicture::animationRotation ()
{
    return rotation();
}

qreal AnimatedItemPicture::animationScale ()
{
    return scale ();
}

void AnimatedItemPicture::setAnimationPos (QPointF value)
{
    setPos(value);
}

void AnimatedItemPicture::setAnimationOpacity (qreal value)
{
    setOpacity(value);
}

void AnimatedItemPicture::setAnimationRotation (qreal value)
{
    setRotation(value);
}

void AnimatedItemPicture::setAnimationScale (qreal value)
{
    setScale(value);
}
