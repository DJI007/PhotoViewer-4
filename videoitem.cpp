#include "videoitem.h"

#include <QGraphicsScene>
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

    _panel = new QGraphicsRectItem(0,
                                   0,
                                   300,
                                   40,
                                   this);

    _panel->setBrush(QColor(150, 150, 150, 127));
    _panel->setParentItem(this);

    QSlider *slider;

    slider = new QSlider (Qt::Horizontal);
    _sliderPosition = new QGraphicsProxyWidget();
    _sliderPosition->setWidget(slider);
    _sliderPosition->setParentItem(_panel);
    connect (_player,
             SIGNAL(positionChanged(qint64)),
             this,
             SLOT(on_positionChanged(qint64)));


    QFile file(":/qss/qss/video_slider.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    slider->setStyleSheet(styleSheet);

/*
    QPushButton *button;

     button = new QPushButton();
     button->setIcon(QIcon(":/images/images/media-playback-start.png"));

     _buttonPlay = new QGraphicsProxyWidget();
     _buttonPlay->setWidget(button);
     _buttonPlay->setParentItem(_panel);
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

        QSlider *position;

        position = (QSlider *) _sliderPosition->widget();
        position->setMinimum(0);
        position->setMaximum(_player->duration() / 1000);
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

void VideoItem::on_positionChanged(qint64 value)
{
    QSlider *position;

    position = (QSlider *) _sliderPosition->widget();
    position->setValue(value / 1000);
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
