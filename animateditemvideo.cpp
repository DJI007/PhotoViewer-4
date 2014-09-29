#include "animateditemvideo.h"

#include <QGraphicsScene>
#include <QDebug>
#include <QFileInfo>
#include <QDateTime>

AnimatedItemVideo::AnimatedItemVideo(QString fileName, QObject *parent) :
    AnimatedItem(parent)
{
    _fileName = fileName;

    _player = new QMediaPlayer (parent);
    _player->setVideoOutput(this);

    _info = NULL;
}

AnimatedItemVideo::~AnimatedItemVideo()
{
    delete _player;
}

void AnimatedItemVideo::load()
{
    _player->setMedia(QUrl::fromLocalFile(_fileName));
    QGraphicsVideoItem::connect (_player,
             SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
             this,
             SLOT(on_mediaStatusChanged(QMediaPlayer::MediaStatus)));
}

void AnimatedItemVideo::endAnimation()
{
    // qDebug () << _player->position() << "-.-" << _player->isSeekable();
    // _player->play();
}

void AnimatedItemVideo::resize ()
{
    qDebug () << "resize: " << scene()->height();
    setSize(QSizeF(this->scene()->width(), this->scene()->height()));
    setChildrenPos();
}

void AnimatedItemVideo::setChildrenPos ()
{
    if (_info != NULL) {
        qreal vPos;
        qreal videoHeight;
        qreal percent;

        percent = ((size().height() * 100) / nativeSize().height());

        videoHeight = ((size().height() * percent) / 100);
        vPos = (size().height() / 2) + (videoHeight / 2);

        // _info->setPos(0, size().height() - 40);
        _info->setPos(5, 5);
        qDebug () << "setChildrenPos: " << nativeSize().height() << "-.-" << size().height() << "-.-" << vPos;
        qDebug () << "setChildrenPos: " << nativeSize().width() << "-.-" << size().width();
    }
}

QGraphicsTextItem *AnimatedItemVideo::createInfo()
{
    QGraphicsTextItem *item;
    QString msg;
    QFileInfo info;

    info.setFile(_fileName);

    msg = "<span style=\"background-color: black; color: white; margin:5px 5px 5px 5px\">";
    msg += info.fileName();
    msg += "<br />";
    msg += info.created().toString(Qt::SystemLocaleLongDate);
    msg += "</span>";

    item = new QGraphicsTextItem();
    item->setHtml(msg);

    return item;
}


void AnimatedItemVideo::setRating(int rating)
{
    // TODO: This doesn't work
    mediaObject()->metaData("UserRating").setValue(rating);
}

double AnimatedItemVideo::latitude()
{
    return 0;
}

double AnimatedItemVideo::longitude()
{
    return 0;
}

void AnimatedItemVideo::setInfoVisible(bool visible)
{
    if (visible) {
        _info->show();
    }
    else {
        _info->hide();
    }
}

QPointF AnimatedItemVideo::animationPos ()
{
    return pos();
}

qreal AnimatedItemVideo::animationOpacity ()
{
    return opacity();
}

qreal AnimatedItemVideo::animationRotation ()
{
    return rotation();
}

qreal AnimatedItemVideo::animationScale ()
{
    return scale ();
}

void AnimatedItemVideo::setAnimationPos (QPointF value)
{
    setPos(value);
}

void AnimatedItemVideo::setAnimationOpacity (qreal value)
{
    setOpacity(value);
}

void AnimatedItemVideo::setAnimationRotation (qreal value)
{
    setRotation(value);
}

void AnimatedItemVideo::setAnimationScale (qreal value)
{
    setScale(value);
}

void AnimatedItemVideo::on_mediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::MediaStatus::LoadedMedia) {
        resize();
        _player->play();

        _info = createInfo ();
        _info->setParentItem(this);

        setChildrenPos();
    }
}
