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

    _emitShowTimeEnded = false;
    _emitItemLoaded = false;

    _player = new QMediaPlayer (parent);
    _player->setVideoOutput(this);
    _panel = new VideoControlPanel(this);

    connect (_player,
             SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
             this,
             SLOT(on_mediaStatusChanged(QMediaPlayer::MediaStatus)));
    connect (this,
             SIGNAL(nativeSizeChanged(QSizeF)),
             this,
             SLOT(on_nativeSizeChanged(QSizeF)));
    connect (_player,
             SIGNAL(stateChanged(QMediaPlayer::State)),
             this,
             SLOT(on_stateChanged(QMediaPlayer::State)));
    connect (_player,
             SIGNAL(volumeChanged(int)),
             this,
             SLOT(on_volumeChanged(int)));

    connect (this, SIGNAL(playMedia()),
             _panel, SLOT(on_play()));
    connect (this, SIGNAL(pauseMedia()),
             _panel, SLOT(on_pause()));
    connect (this, SIGNAL(stopMedia()),
             _panel, SLOT(on_stop()));

    connect (_player, SIGNAL(positionChanged(qint64)),
             _panel, SLOT(on_positionChanged(qint64)));
    connect (_panel, SIGNAL(positionChanged(qint64)),
             _player, SLOT(setPosition(qint64)));
    connect (_panel, SIGNAL(playClicked()),
             _player, SLOT(play()));
    connect (_panel, SIGNAL(pauseClicked()),
             _player, SLOT(pause()));
    connect (_panel, SIGNAL(volumeChanged(int)),
             _player, SLOT(setVolume(int)));
}

VideoItem::~VideoItem()
{
    if (_videoData != NULL)
        delete _videoData;
}

void VideoItem::load(bool fireEvent)
{
    _emitItemLoaded = fireEvent;
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

        if (_emitItemLoaded) {
            emit itemLoaded();
        }

        _panel->setVolume(_player->volume());
        _panel->setDuration (_player->duration());
    }
    else if (status == QMediaPlayer::EndOfMedia) {
        if (_emitShowTimeEnded) {
            emit showTimeEnded();
        }
//        _player->setPosition(_player->duration() / 2);
//        _player->pause();
    }
}

void VideoItem::on_videoAvailableChanged(bool available)
{
    qDebug () << "Video available";
}

void VideoItem::on_stateChanged(QMediaPlayer::State state)
{
    if (state == QMediaPlayer::PlayingState) {
        emit playMedia();
    }
    else if (state == QMediaPlayer::PausedState) {
        emit pauseMedia();
    }
    else if (state == QMediaPlayer::StoppedState) {
        emit stopMedia();
    }
}

void VideoItem::on_nativeSizeChanged(const QSizeF &size)
{
    if (size.width() > 0) {
        resize();
        if (_emitItemLoaded) {
            emit itemLoaded();
        }
    }
}

void VideoItem::on_volumeChanged(int volume)
{
    emit volumeChanged (volume);
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

void VideoItem::createControls()
{
    _panel->setPos (this->boundingRect().right() - (_panel->rect().width() + 10),
                    this->boundingRect().bottom() - (_panel->rect().height() + 10));
}

void VideoItem::setShowTime(int time)
{
    _emitShowTimeEnded = (time > 0);
}

bool VideoItem::rotateLeft()
{
    // TODO: Rotate video
    return false;
}

bool VideoItem::rotateRight()
{
    // TODO: Rotate video
    return false;
}
