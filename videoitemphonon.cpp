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

    _player = new Phonon::MediaObject();
    _player->setTickInterval(1000);

    _audio = new Phonon::AudioOutput(Phonon::VideoCategory);
    _video = new Phonon::VideoWidget();
    _video->setAspectRatio(Phonon::VideoWidget::AspectRatio16_9);
    _video->setScaleMode(Phonon::VideoWidget::FitInView);

    Phonon::createPath(_player, _audio);
    Phonon::createPath(_player, _video);

    this->setWidget(_video);
    this->setPos(0, 0);

    createPanel();

    connect (_player, SIGNAL(tick(qint64)),
             _panel, SLOT(setPosition(qint64)));
    connect (_player, SIGNAL(aboutToFinish()),
             this, SLOT(on_aboutToFinish()));
    connect (_player, SIGNAL(totalTimeChanged(qint64)),
             this, SLOT(on_durationChanged (qint64)));
    connect (_player, SIGNAL(stateChanged(Phonon::State,Phonon::State)),
             this, SLOT(on_stateChanged (Phonon::State,Phonon::State)));

    connect (_audio, SIGNAL(volumeChanged(qreal)),
             _panel, SLOT(setVolume(qreal)));
}

VideoItemPhonon::~VideoItemPhonon()
{
    if (_videoData)
        delete _videoData;

    SettingsHelper::instance().setVolume(_audio->volume());

    // Only delete audio, video is deleted by player
    delete _audio;
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
    }
}

void VideoItemPhonon::on_beginItemAnimationIn()
{
    _player->pause();
}

void VideoItemPhonon::on_endItemAnimationIn()
{
    _player->play();
}

void VideoItemPhonon::on_beginItemAnimationOut()
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


    connect (this, SIGNAL(playMedia()),
             _panel, SLOT(play()));
    connect (this, SIGNAL(pauseMedia()),
             _panel, SLOT(pause()));
    connect (this, SIGNAL(stopMedia()),
             _panel, SLOT(stop()));

    connect (_panel, SIGNAL(positionChanged(qint64)),
             _player, SLOT(seek(qint64)));
    connect (_panel, SIGNAL(playClicked()),
             _player, SLOT(play()));
    connect (_panel, SIGNAL(pauseClicked()),
             _player, SLOT(pause()));

    connect (_panel, SIGNAL(volumeChanged(qreal)),
             _audio, SLOT(setVolume(qreal)));
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
    // TODO: Rotate video
    return true;
}

bool VideoItemPhonon::rotateRight()
{
    // TODO: Rotate video
    return true;
}
