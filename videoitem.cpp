#include "videoitem.h"

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPushButton>
#include <QSlider>
#include <QFileInfo>
#include <QDateTime>
#include <QMediaMetaData>

#include "abstractmetadata.h"
#include "settingshelper.h"

VideoItem::VideoItem(QString fileName, QObject *parent)
{
    this->setParent(parent);

    _fileName = fileName;
    _videoData = new XMPMetadata(fileName);

    _emitShowTimeEnded = false;

    _player = new QMediaPlayer (this);
    _player->setVideoOutput(this);

    createPanel();

    connect (_player, SIGNAL(positionChanged(qint64)),
             this, SLOT(on_positionChanged(qint64)));
    connect (_player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
             this, SLOT(on_mediaStatusChanged(QMediaPlayer::MediaStatus)));
    connect (this, SIGNAL(nativeSizeChanged(QSizeF)),
             this, SLOT(on_nativeSizeChanged(QSizeF)));
    connect (_player, SIGNAL(stateChanged(QMediaPlayer::State)),
             this, SLOT(on_stateChanged(QMediaPlayer::State)));
    connect (_player, SIGNAL(volumeChanged(int)),
             this, SLOT(on_volumeChanged(int)));
}

VideoItem::~VideoItem()
{
    SettingsHelper::instance().setVolume(_player->volume());
    if (_videoData)
        delete _videoData;
}

void VideoItem::load()
{
    _player->setMedia(QUrl::fromLocalFile(_fileName));
    _player->setVolume(SettingsHelper::instance().volume());

    // _panel->setVolume(_player->volume());
    // _panel->show();
}

void VideoItem::resize()
{
    setSize(QSizeF(this->scene()->width(), this->scene()->height()));
    setPanelPosition ();
}

QDateTime VideoItem::getDate()
{
    QDateTime result;

    result = _videoData->pictureDate();
    if (!result.isValid()) {
        QFileInfo info;

        info.setFile(_fileName);

        result = info.created();
    }

    return result;
}

void VideoItem::on_mediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    qDebug () << "on media status changed: " << status;
    if (status == QMediaPlayer::MediaStatus::LoadedMedia) {
        _player->play();

        resize();

        if (_panel) {
            _panel->setVolume(_player->volume());
            _panel->setDuration (_player->duration());
        }
        //_panel->show();
    }
    else if (status == QMediaPlayer::MediaStatus::BufferedMedia) {
        // Linux doesn't fires the LoadedMedia signal
        if (_panel) {
            _panel->setDuration (_player->duration());
        }

        qDebug () << _player->metaData(QMediaMetaData::UserRating).toString();
    }
    else if (status == QMediaPlayer::EndOfMedia) {
        if (_emitShowTimeEnded) {
            emit showTimeEnded();
        }
    }
}

void VideoItem::on_stateChanged(QMediaPlayer::State state)
{
    // qDebug () << "on media state changed: " << state;
    if (state == QMediaPlayer::PlayingState) {
        //_panel->show();
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

        _panel->show();

        emit itemLoaded();
    }
}

void VideoItem::on_positionChanged(qint64 value)
{
    if (value == _player->duration()) {
    }

    if (_panel) {
        _panel->setPosition (value);
    }
}

void VideoItem::on_volumeChanged(int volume)
{
    emit volumeChanged (volume);
}

void VideoItem::on_beginItemAnimationIn()
{
    _player->pause();
}

void VideoItem::on_endItemAnimationIn()
{
    _player->play();
}

void VideoItem::on_beginItemAnimationOut()
{
    _player->pause();
}

AbstractMetadata *VideoItem::metadata()
{
    return _videoData;
}

void VideoItem::createPanel()
{
    _panel = new VideoControlPanel(this);
    _panel->setParentItem(this);
    setPanelPosition();

    _panel->setVolume(_player->volume());
    // _panel->show();
    _panel->hide();

    connect (this, SIGNAL(playMedia()),
             _panel, SLOT(on_play()));
    connect (this, SIGNAL(pauseMedia()),
             _panel, SLOT(on_pause()));
    connect (this, SIGNAL(stopMedia()),
             _panel, SLOT(on_stop()));

    connect (_panel, SIGNAL(positionChanged(qint64)),
             _player, SLOT(setPosition(qint64)));
    connect (_panel, SIGNAL(playClicked()),
             _player, SLOT(play()));
    connect (_panel, SIGNAL(pauseClicked()),
             _player, SLOT(pause()));
    connect (_panel, SIGNAL(volumeChanged(int)),
             _player, SLOT(setVolume(int)));
}

void VideoItem::setPanelPosition()
{
    if (_panel) {
        _panel->setPos (this->boundingRect().right() - (_panel->rect().width() + 10),
                        this->boundingRect().bottom() - (_panel->rect().height() + 10));
    }
}

void VideoItem::setShowTime(int time)
{
    _emitShowTimeEnded = (time > 0);
}

bool VideoItem::rotateLeft()
{
    // TODO: Rotate video
    return true;
}

bool VideoItem::rotateRight()
{
    // TODO: Rotate video
    return true;
}
