#include "videoitemphonon.h"

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPushButton>
#include <QSlider>
#include <QFileInfo>
#include <QDateTime>

#include "abstractmetadata.h"
#include "settingshelper.h"

VideoItemPhonon::VideoItemPhonon(QString fileName, QObject *parent)
{
    setParent(parent);

    _fileName = fileName;
    _videoData = new XMPMetadata(fileName);

    _emitShowTimeEnded = false;

    _player = new Phonon::MediaObject(parent);
    _player->setTickInterval(1000);

    _audio = new Phonon::AudioOutput(Phonon::VideoCategory);
    _video = new Phonon::VideoWidget();
    _video->setAspectRatio(Phonon::VideoWidget::AspectRatio16_9);
    _video->setScaleMode(Phonon::VideoWidget::FitInView);

    Phonon::createPath(_player, _audio);
    Phonon::createPath(_player, _video);

    this->setWidget(_video);
    this->setPos(0, 0);

    _controller = new Phonon::MediaController (_player);

    createPanel();

    connect (_controller, SIGNAL(availableSubtitlesChanged()),
             this, SLOT(on_availableSubitlesChanged ()));

    _rotateFilter = new VideoFilter();
    connect (_rotateFilter, SIGNAL(rotateDone()),
             this, SLOT(on_videoFilterFinished()));
}

VideoItemPhonon::~VideoItemPhonon()
{
    if (_videoData)
        delete _videoData;

    SettingsHelper::instance().setVolume(_audio->volume());

    // Only delete audio, video is deleted by player
    delete _audio;

    delete _controller;

    delete _rotateFilter;
}

void VideoItemPhonon::load()
{
    _player->setCurrentSource(Phonon::MediaSource(QUrl::fromLocalFile(_fileName)));

    resize();

    _panel->setVolume(_audio->volume());
    _panel->show();

    emit itemLoaded();
}

void VideoItemPhonon::resize()
{
    _video->resize(this->scene()->width(), this->scene()->height());
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
    _panel = new VideoControlPanel();
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
    _rotateFilter->transposeAsync(_fileName, VideoFilter::RotateDirection::CounterClockWise);

    qDebug () << "Rotate left done!!";

    return true;
}

bool VideoItemPhonon::rotateRight()
{
    return true;
}

void VideoItemPhonon::beginRotateLeftAnimation()
{
    qDebug () << "BeginRotateLeftAnimation";
    _player->pause();
    _panel->hide();
}

void VideoItemPhonon::endRotateLeftAnimation()
{
    qDebug () << "EndRotateLeftAnimation: Calling rotate left";
    //_rotateFilter->rotate(_fileName, VideoFilter::RotateDirection::CounterClockWise);

    load ();
    // setPanelPosition();
    // _panel->show();
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
    _rotateFilter->rotate(_fileName, VideoFilter::RotateDirection::ClockWise);

    // _panel->show();
    // setPanelPosition();
}

void VideoItemPhonon::on_videoFilterFinished()
{
    qDebug () << "Rotate done";
    //load ();
}

