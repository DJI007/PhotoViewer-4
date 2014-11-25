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
    setBrush(Qt::NoBrush);
    setPen(Qt::NoPen);

    try {
        _videoData = new XMPMetadata(fileName);
    }
    catch (Exiv2::AnyError& e) {
        Q_UNUSED(e);
        _videoData = new SQLiteMetadata (fileName);
    }

    _emitShowTimeEnded = false;

    _player = new Phonon::MediaObject(this);
    _player->setTickInterval(1000);
    _controller = new Phonon::MediaController (_player);
    connect (_controller, SIGNAL(availableSubtitlesChanged()),
             this, SLOT(on_availableSubitlesChanged ()));

    _audio = new Phonon::AudioOutput(Phonon::VideoCategory);
    _video = new Phonon::VideoWidget();
    _video->setAspectRatio(Phonon::VideoWidget::AspectRatio16_9);
    _video->setScaleMode(Phonon::VideoWidget::FitInView);

    Phonon::createPath(_player, _audio);
    Phonon::createPath(_player, _video);

    _videoItem = new QGraphicsProxyWidget(this);
    _videoItem->setWidget(_video);
    _videoItem->setPos(0, 0);

    createPanel();
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

    setItemRotation(_videoData->orientation());
    resize();

    _panel->setVolume(_audio->volume());
    _panel->show();

    emit itemLoaded();
}

void VideoItemPhonon::resize()
{
    // this->setRect(0, 0, this->scene()->width(), this->scene()->height());
    int rot;
    qreal width;
    qreal height;
    qreal sceneWidth;
    qreal sceneHeight;
    qreal scaleFactor;
    qreal left;
    qreal top;

    rot = qAbs(((int) itemRotation()) % 360);

    sceneHeight = this->scene()->height();
    sceneWidth = this->scene()->width();
    if (rot == 90 || rot == 270) {
        width = sceneHeight;
        height = sceneWidth;
    }
    else {
        width = this->scene()->width();
        height = this->scene()->height();
    }

    scaleFactor = sceneHeight / height;
    if ((width * scaleFactor) > sceneWidth) {
        scaleFactor = sceneWidth / width;
    }

    left = 0;
    top = 0;

    this->setRect(left, top, width, height);

    _video->resize(sceneWidth, sceneHeight);
    _videoItem->setTransformOriginPoint(sceneWidth / 2, sceneHeight / 2);
    if (_videoItem->scale() != scaleFactor) {
        _videoItem->setScale(scaleFactor);
    }

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
        if (_emitShowTimeEnded && oldState == Phonon::PlayingState) {
            emit showTimeEnded();
        }

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
    // qDebug () << _controller->availableSubtitles().count() << "-.-" << _controller->subtitleAutodetect();
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
    if (_panel && this->scene()) {
        /*
        _panel->setPos (this->boundingRect().right() - (_panel->rect().width() + 10),
                        this->boundingRect().bottom() - (_panel->rect().height() + 10));
        */
        _panel->setPos (this->scene()->width() - (_panel->rect().width() + 10),
                        this->scene()->height() - (_panel->rect().height() + 10));

    }
}

void VideoItemPhonon::setShowTime(int time)
{
    _emitShowTimeEnded = (time > 0);
}

bool VideoItemPhonon::rotateLeft()
{
    return true;
}

bool VideoItemPhonon::rotateRight()
{
    return true;
}

void VideoItemPhonon::beginRotateAnimation()
{
    _player->stop();
    _panel->hide();
}

void VideoItemPhonon::endRotateAnimation()
{
    // Change width and height
    // this->setRect(0, 0, this->rect().height(), this->rect().width());
    _videoData->setOrientation(((int) itemRotation()) % 360);

    resize ();

    _panel->show();
    _player->play();
}

void VideoItemPhonon::setItemRotation(qreal angle)
{
    _videoItem->setRotation(angle);
}

qreal VideoItemPhonon::itemRotation()
{
    return _videoItem->rotation();
}

void VideoItemPhonon::setItemScale(qreal scale)
{
    _videoItem->setScale(scale);
}

qreal VideoItemPhonon::itemScale()
{
    return _videoItem->scale();
}

