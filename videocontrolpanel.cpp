
#include <QSlider>
#include <QPushButton>
#include <QFile>
#include <QPixmap>
#include <QGraphicsPixmapItem>

#include "videocontrolpanel.h"

VideoControlPanel::VideoControlPanel(QGraphicsItem *parent) :
    QGraphicsRectItem(0, 0, 200, 35, parent)
{
    setBrush(QColor(100, 100, 100, 175));
    setParentItem(this);

    QSlider *slider;

    slider = new QSlider (Qt::Horizontal);

    _sliderPosition = new QGraphicsProxyWidget();
    _sliderPosition->setWidget(slider);
    _sliderPosition->setParentItem(this);

    slider->setStyleSheet(readCSS(":/qss/qss/video_slider.css"));
    slider->resize(this->boundingRect().width() - 10, slider->height());
    slider->move(5, 0);

    _buttonRew = addButtonToPanel(":/images/images/media-seek-backward.png",
                                   ":/images/images/media-seek-backward.png",
                                   5,
                                  false);
    _buttonStop = addButtonToPanel(":/images/images/media-playback-stop.png",
                                   ":/images/images/media-playback-stop.png",
                                   20,
                                   false);
    _buttonPlay = addButtonToPanel(":/images/images/media-playback-pause.png",
                                   ":/images/images/media-playback-start.png",
                                   35,
                                   true);
    _buttonFF = addButtonToPanel(":/images/images/media-seek-forward.png",
                                 ":/images/images/media-seek-forward.png",
                                 50,
                                 false);

    slider = new QSlider (Qt::Horizontal);

    _sliderVolume = new QGraphicsProxyWidget();
    _sliderVolume->setWidget(slider);
    _sliderVolume->setParentItem(this);

    slider->setStyleSheet(readCSS(":/qss/qss/video_slider.css"));
    slider->resize(50, slider->height());
    slider->move(this->boundingRect().width() - (slider->width() + 5),
                 this->boundingRect().height() - (slider->height() + 3));
    slider->setMinimum(0);
    slider->setMaximum(100);

    QGraphicsPixmapItem *volumeIcon;
    QPixmap volumePixmap (":/images/images/audio-volume-high.png");

    volumeIcon = new QGraphicsPixmapItem(volumePixmap.scaled(18, 18, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    volumeIcon->setParentItem(this);
    volumeIcon->setPos(slider->pos().x() - (volumeIcon->boundingRect().width() + 3),
                       this->boundingRect().height() - (volumeIcon->boundingRect().height() + 5));

    // Register signals
    connect (((QSlider *) _sliderPosition->widget()), SIGNAL(sliderMoved(int)),
             this, SIGNAL(positionChanged(int)));

    connect (((QPushButton *) _buttonRew->widget()), SIGNAL(clicked()),
             this, SIGNAL(rewindClicked()));

    connect (((QPushButton *) _buttonStop->widget()), SIGNAL(clicked()),
             this, SIGNAL(stopClicked()));

    connect (((QPushButton *) _buttonPlay->widget()), SIGNAL(toggled(bool)),
             this, SIGNAL(playToggled (bool)));

    connect (((QPushButton *) _buttonFF->widget()), SIGNAL(clicked()),
             this, SIGNAL(fastForwardClicked()));

    connect (((QSlider *) _sliderVolume->widget()), SIGNAL(sliderMoved(int)),
             this, SIGNAL(volumeChanged(int)));

}

QGraphicsProxyWidget *VideoControlPanel::addButtonToPanel(QString iconPathOn,
                                                          QString iconPathOff,
                                                          int posX,
                                                          bool checkable)
{
    QGraphicsProxyWidget *result;
    QPushButton *button;
    QIcon icon;

    icon.addPixmap(QPixmap(iconPathOn), QIcon::Normal, QIcon::On);
    icon.addPixmap(QPixmap(iconPathOff), QIcon::Normal, QIcon::Off);

    button = new QPushButton();
    button->setIcon(icon);
    button->setFlat(true);
    if (checkable) {
        button->setCheckable(true);
    }
    button->setStyleSheet("background-color:transparent;");
    button->resize(25, 25);
    button->move(posX, this->boundingRect().height() - button->height());

    result = new QGraphicsProxyWidget();
    result->setWidget(button);
    result->setParentItem(this);

    return result;
}

void VideoControlPanel::on_positionChanged(qint64 value)
{
    QSlider *position;

    position = (QSlider *) _sliderPosition->widget();
    position->setValue(value / 1000);
}

/*
void VideoControlPanel::on_buttonPlayToggled(bool checked)
{
    if (checked) {
        emit pauseButtonClicked();
    }
    else {
        emit playButtonClicked();
    }

}
*/

void VideoControlPanel::setMaximum(int value)
{
    QSlider *position;

    position = (QSlider *) _sliderPosition->widget();
    position->setMaximum(value);
}

void VideoControlPanel::setMinimum(int value)
{
    QSlider *position;

    position = (QSlider *) _sliderPosition->widget();
    position->setMinimum(value);
}

QString VideoControlPanel::readCSS(QString path)
{
    QFile file (path);

    file.open(QFile::ReadOnly);

    return QLatin1String(file.readAll());
}

void VideoControlPanel::setVolume(int value)
{
    QSlider *volume;

    volume = (QSlider *) _sliderVolume->widget();
    volume->setValue(value);
}
