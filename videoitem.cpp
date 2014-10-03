#include "videoitem.h"

#include <QGraphicsScene>

#include "abstractmetadata.h"

VideoItem::VideoItem(QString fileName, QObject *parent)
{
    _fileName = fileName;
    _videoData = new ExifMetadata(fileName);

    _player = new QMediaPlayer (parent);
    _player->setVideoOutput(this);
    connect (_player,
             SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
             this,
             SLOT(on_mediaStatusChanged(QMediaPlayer::MediaStatus)));
    connect (this,
             SIGNAL(nativeSizeChanged(QSizeF)),
             this,
             SLOT(on_nativeSizeChanged(QSizeF)));

    /*
    connect (_player,
             SIGNAL(stateChanged(QMediaPlayer::State)),
             this,
             SLOT(on_stateChanged(QMediaPlayer::State)));

    connect (_player,
             SIGNAL(videoAvailableChanged(bool)),
             this,
             SLOT(on_videoAvailableChanged (bool)));
*/
}

VideoItem::~VideoItem()
{
    if (_videoData != NULL)
        delete _videoData;
}

void VideoItem::load()
{
    _player->setMedia(QUrl::fromLocalFile(_fileName));
}

void VideoItem::resize()
{
    setSize(QSizeF(this->scene()->width(), this->scene()->height()));
}

QDateTime VideoItem::getDate()
{
    return QDateTime::currentDateTime();
}

void VideoItem::on_mediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    qDebug () << "on media status changed: " << status;
    if (status == QMediaPlayer::MediaStatus::LoadedMedia) {
        // resize();
        _player->play();
        _player->pause();
    }
}

void VideoItem::on_videoAvailableChanged(bool available)
{
    if (available) {
        emit itemLoaded();
        resize();
        _player->play();
    }
}

void VideoItem::on_stateChanged(QMediaPlayer::State state)
{
    qDebug () << "on state changed: " << state;
/*
    if (state == QMediaPlayer::PlayingState) {
        emit itemLoaded();
        resize();
        _player->play();
    }
*/
}

void VideoItem::on_nativeSizeChanged(const QSizeF &size)
{
    qDebug () << "on native size changed: " << size;
    if (size.width() > 0) {
        resize();
        emit itemLoaded();
        // _player->play();
    }
}

void VideoItem::on_beginItemAnimation()
{
    _player->pause();
}

void VideoItem::on_endItemAnimation()
{
    _player->play();
}

AbstractMetadata *VideoItem::metadata()
{
    return _videoData;
}
