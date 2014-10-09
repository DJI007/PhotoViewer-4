#include "videoitem.h"

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPushButton>
#include <QSlider>

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
    _panel = new VideoControlPanel(this);
    connect (_player, SIGNAL(positionChanged(qint64)),
             _panel, SLOT(on_positionChanged(qint64)));

    connect (_panel, SIGNAL(positionChanged(int)),
             _player, SLOT(setPosition(qint64)));
    connect (_panel, SIGNAL(rewindClicked()),
             this, SLOT(on_rewindClicked ()));
    connect (_panel, SIGNAL(stopClicked()),
             _player, SLOT(stop()));
    connect (_panel, SIGNAL(playToggled(bool)),
             this, SLOT(on_playToggled(bool)));
    connect (_panel, SIGNAL(fastForwardClicked()),
             this, SLOT(on_fastForwardClicked ()));
    connect (_panel, SIGNAL(volumeChanged(int)),
             _player, SLOT(setVolume(int)));
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
    createControls ();
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

        resize();
        emit itemLoaded();

        _panel->setMinimum(0);
        _panel->setMaximum(_player->duration() / 1000);
    }
    else if (status == QMediaPlayer::EndOfMedia) {
//        _player->setPosition(_player->duration() / 2);
//        _player->pause();
    }
}

void VideoItem::on_videoAvailableChanged(bool available)
{
/*
    if (available) {
        _player->play();
    }
*/
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

void VideoItem::on_rewindClicked()
{
    if (_player->playbackRate() > 1) {
        _player->setPlaybackRate(-1);
    }
    else {
        _player->setPlaybackRate(_player->playbackRate() - 1);
    }
}

void VideoItem::on_fastForwardClicked()
{
    if (_player->playbackRate() < 1) {
        _player->setPlaybackRate(2);
    }
    else {
        _player->setPlaybackRate(_player->playbackRate() + 1);
    }
}

void VideoItem::on_playToggled(bool)
{
    qDebug () << "On playToggled: " << _player->state() << "-.-" << _player->playbackRate();
    if (_player->state() == QMediaPlayer::PlayingState) {
        if (_player->playbackRate() != 1) {
            _player->setPlaybackRate(1);
        }
        else {
            _player->pause();
        }
    }
    else {
        _player->play();
    }
}

AbstractMetadata *VideoItem::metadata()
{
    return _videoData;
}

void VideoItem::createControls()
{
    _panel->setPos (this->boundingRect().right() - (_panel->rect().width() + 10),
                    this->boundingRect().bottom() - (_panel->rect().height() + 10));
}

