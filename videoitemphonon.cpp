#include "videoitemphonon.h"

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPushButton>
#include <QSlider>
#include <QFileInfo>
#include <QDateTime>

#include "settingshelper.h"
#include "xmpmetadata.h"
#include "sqlitemetadata.h"

VideoItemPhonon::VideoItemPhonon(QString fileName, QObject *parent)
{
    setParent(parent);

    _fileName = fileName;

    try {
        _videoData = new XMPMetadata(fileName);
    }
    catch (Exiv2::AnyError& e) {
        _videoData = new SQLiteMetadata (fileName);
    }

    _emitShowTimeEnded = false;

    _player = new Phonon::MediaObject(this);
    _player->setTickInterval(1000);

    _audio = new Phonon::AudioOutput(Phonon::VideoCategory);
    _video = new Phonon::VideoWidget();
    _video->setAspectRatio(Phonon::VideoWidget::AspectRatio16_9);
    _video->setScaleMode(Phonon::VideoWidget::FitInView);

    Phonon::createPath(_player, _audio);
    Phonon::createPath(_player, _video);

    _videoItem = new QGraphicsProxyWidget(this);
    _videoItem->setWidget(_video);
    _videoItem->setPos(0, 0);

    _controller = new Phonon::MediaController (_player);

    createPanel();

    connect (_controller, SIGNAL(availableSubtitlesChanged()),
             this, SLOT(on_availableSubitlesChanged ()));
}

VideoItemPhonon::~VideoItemPhonon()
{
    if (_videoData)
        delete _videoData;

    SettingsHelper::instance().setVolume(_audio->volume());

    // Only delete audio, video is deleted by player
    delete _audio;

    delete _controller;
}

void VideoItemPhonon::load()
{
    _player->setCurrentSource(Phonon::MediaSource(QUrl::fromLocalFile(_fileName)));

    this->setRect(0, 0, this->scene()->width(), this->scene()->height());

    resize();

    _panel->setVolume(_audio->volume());
    _panel->show();

    emit itemLoaded();
}

void VideoItemPhonon::resize()
{
    this->setRect(0, 0, this->scene()->width(), this->scene()->height());
    _video->resize(this->scene()->width(), this->scene()->height());
    // _videoItem->setTransformOriginPoint(this->boundingRect().width() / 2, this->boundingRect().height() / 2);
    _videoItem->setTransformOriginPoint(_video->width() / 2, _video->height() / 2);
    setPanelPosition ();
}

QDateTime VideoItemPhonon::getDate()
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

void VideoItemPhonon::on_aboutToFinish()
{
    if (_emitShowTimeEnded) {
        emit showTimeEnded();
    }
}

void VideoItemPhonon::on_durationChanged(qint64 value)
{
    if (value != -1) {
        _panel->setDuration(value);
    }
}

void VideoItemPhonon::on_stateChanged(Phonon::State newState, Phonon::State oldState)
{
    Q_UNUSED(oldState);

    switch (newState) {
    case Phonon::PlayingState:
        emit playMedia();
        break;

    case Phonon::PausedState:
        emit pauseMedia();
        break;

    case Phonon::StoppedState:
        emit stopMedia();
        break;

    case Phonon::LoadingState:
    case Phonon::BufferingState:
    case Phonon::ErrorState:
        break;
    }
}

void VideoItemPhonon::on_availableSubitlesChanged()
{
    qDebug () << _controller->availableSubtitles().count() << "-.-" << _controller->subtitleAutodetect();
}

void VideoItemPhonon::beginItemAnimationIn()
{
    _player->pause();
}

void VideoItemPhonon::endItemAnimationIn()
{
    _player->play();
}

void VideoItemPhonon::beginItemAnimationOut()
{
    _player->pause();
}

AbstractMetadata *VideoItemPhonon::metadata()
{
    return _videoData;
}

void VideoItemPhonon::createPanel()
{
    _panel = new VideoControlPanel(this);
    _panel->setParentItem(this);
    setPanelPosition();

    _panel->setVolume(_audio->volume());
    _panel->hide();

    // this -> panel
    connect (this, SIGNAL(playMedia()),
             _panel, SLOT(play()));
    connect (this, SIGNAL(pauseMedia()),
             _panel, SLOT(pause()));
    connect (this, SIGNAL(stopMedia()),
             _panel, SLOT(stop()));

    // panel -> player
    connect (_panel, SIGNAL(positionChanged(qint64)),
             _player, SLOT(seek(qint64)));
    connect (_panel, SIGNAL(playClicked()),
             _player, SLOT(play()));
    connect (_panel, SIGNAL(pauseClicked()),
             _player, SLOT(pause()));

    // panel -> player (audio)
    connect (_audio, SIGNAL(volumeChanged(qreal)),
             _panel, SLOT(setVolume(qreal)));

    // player -> panel
    connect (_player, SIGNAL(tick(qint64)),
             _panel, SLOT(setPosition(qint64)));
    connect (_player, SIGNAL(aboutToFinish()),
             this, SLOT(on_aboutToFinish()));
    connect (_player, SIGNAL(totalTimeChanged(qint64)),
             this, SLOT(on_durationChanged (qint64)));
    connect (_player, SIGNAL(stateChanged(Phonon::State,Phonon::State)),
             this, SLOT(on_stateChanged (Phonon::State,Phonon::State)));
}

void VideoItemPhonon::setPanelPosition()
{
    if (_panel) {
        _panel->setPos (this->boundingRect().right() - (_panel->rect().width() + 10),
                        this->boundingRect().bottom() - (_panel->rect().height() + 10));
    }
}

void VideoItemPhonon::setShowTime(int time)
{
    _emitShowTimeEnded = (time > 0);
}

bool VideoItemPhonon::rotateLeft()
{
    _player->clear();

    qDebug () << "Rotate left done!!";

    return true;
}

bool VideoItemPhonon::rotateRight()
{
    return true;
}

void VideoItemPhonon::beginRotateLeftAnimation()
{
    qDebug () << "BeginRotateLeftAnimation: " << rotation();
    _player->stop();
    _panel->hide();
}

void VideoItemPhonon::endRotateLeftAnimation()
{
    qDebug () << "EndRotateLeftAnimation: Calling rotate left: " << rotation();

    load ();
    // setPanelPosition();
    _panel->show();
    _player->play();
}

void VideoItemPhonon::beginRotateRightAnimation()
{
    qDebug () << "BeginRotateRightAnimation";
    _player->pause();
    _panel->hide();
}

void VideoItemPhonon::endRotateRightAnimation()
{
    _player->clear();

    qDebug () << "EndRotateRightAnimation: Calling rotate right";

    _panel->show();
    // setPanelPosition();
}

void VideoItemPhonon::setItemRotation(qreal angle)
{
    qDebug () << "VideoItemPhonon::setRotation: " << angle;

    //QGraphicsRectItem::setRotation(angle);
    _videoItem->setRotation(angle);
}

qreal VideoItemPhonon::itemRotation()
{
    qDebug () << "VideoItemPhonon::rotation";

    return _videoItem->rotation();
}

void VideoItemPhonon::setItemScale(qreal scale)
{
    qDebug () << "VideoItemPhonon::setScale: " << scale;
    //QGraphicsRectItem::setScale(scale);
    _videoItem->setScale(scale);
}

qreal VideoItemPhonon::itemScale()
{
    qDebug () << "VideoItemPhonon::scale";
    return _videoItem->scale();
}

