
#include <QSlider>
#include <QPushButton>
#include <QFile>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QDebug>

#include "videocontrolpanel.h"

VideoControlPanel::VideoControlPanel(QGraphicsItem *parent) :
    QGraphicsRectItem(0, 0, 200, 35, parent)
{
    setBrush(QColor(100, 100, 100, 175));
    setParentItem(parent);

    _sliderPosition = createSlider(20,
                                   1,
                                   this->boundingRect().width() - 25);
    _buttonPlay = createButton(":/images/images/media-playback-pause-small.png",
                               ":/images/images/media-playback-start-small.png");
    _sliderVolume = createSlider (this->boundingRect().width() - 55,
                                  this->boundingRect().height() - 20,
                                  50);

    QSlider *slider;

    slider = (QSlider *) _sliderVolume->widget();

    slider->setMaximum(100);

    createIcon (":/images/images/audio-volume-high.png",
                slider->pos().x() - 20,
                this->boundingRect().height() - 20,
                16,
                16);

    _textInfo = createText (2, this->boundingRect().height() - 22, "00:00:00/00:00:00");

    // Register signals
    connect (((QSlider *) _sliderPosition->widget()), SIGNAL(actionTriggered(int)),
             this, SLOT(on_sliderPositionActionTriggered(int)));
    connect (((QSlider *) _sliderPosition->widget()), SIGNAL(sliderMoved(int)),
             this, SLOT(on_sliderPositionMoved(int)));

    connect (((QPushButton *) _buttonPlay->widget()), SIGNAL(clicked(bool)),
             this, SLOT(on_playClicked(bool)));

    connect (((QSlider *) _sliderVolume->widget()), SIGNAL(sliderMoved(int)),
             this, SIGNAL(volumeChanged(int)));

}

QGraphicsProxyWidget *VideoControlPanel::createButton(QString iconPathOn, QString iconPathOff)
{
    QGraphicsProxyWidget *result;
    QPushButton *button;
    QIcon icon;

    icon.addPixmap(QPixmap(iconPathOn), QIcon::Normal, QIcon::On);
    icon.addPixmap(QPixmap(iconPathOff), QIcon::Normal, QIcon::Off);

    button = new QPushButton();
    button->setIcon(icon);
    button->setFlat(true);
    button->setCheckable(true);
    button->setStyleSheet("background-color:transparent;");
    button->resize(20, 20);
    button->move(0, 0);

    result = new QGraphicsProxyWidget(this);
    result->setWidget(button);
    result->setParentItem(this);

    return result;
}

QGraphicsProxyWidget *VideoControlPanel::createSlider(int x, int y, int width)
{
    QSlider *slider;
    QGraphicsProxyWidget *result;

    slider = new QSlider (Qt::Horizontal);

    result = new QGraphicsProxyWidget(this);
    result->setWidget(slider);
    result->setParentItem(this);

    slider->setStyleSheet(readCSS(":/qss/qss/video_slider.css"));
    slider->setMinimum(0);
    slider->resize(width, slider->height());
    slider->move(x, y);
    slider->setSingleStep(1);
    slider->setPageStep(5);

    return result;
}

void VideoControlPanel::createIcon (QString file, int x, int y, int width, int height)
{
    QGraphicsPixmapItem *icon;
    QPixmap pixmap (file);

    icon = new QGraphicsPixmapItem(pixmap.scaled(width,
                                                 height,
                                                 Qt::KeepAspectRatio,
                                                 Qt::SmoothTransformation));
    icon->setParentItem(this);
    icon->setPos(x, y);
}

QGraphicsTextItem *VideoControlPanel::createText (int x, int y, QString text)
{
    QGraphicsTextItem *result;

    result = new QGraphicsTextItem (text, this);
    result->setDefaultTextColor(Qt::white);
    result->setPos(x, y);
    result->setParentItem(this);

    return result;
}

void VideoControlPanel::on_positionChanged(qint64 value)
{
    QSlider *position;
    QTime time(0, 0, 0, 0);
    QString format;

    position = (QSlider *) _sliderPosition->widget();
    position->setValue(value / 1000);

    time = time.addMSecs(value);

    if (_duration.hour() > 0) {
        format = "hh:mm:sss";
    }
    else {
        format = "mm:ss";
    }

    _textInfo->setPlainText(time.toString(format).append("/").append(_duration.toString(format)));
}

void VideoControlPanel::on_sliderPositionActionTriggered(int action)
{
    QSlider *slider;
    int value;
    bool emitSignal;

    slider = (QSlider *) _sliderPosition->widget();
    value = slider->value();

    emitSignal = false;

    switch (action) {
    case QAbstractSlider::SliderPageStepSub:
        value -= slider->pageStep();
        emitSignal = true;
        break;
    case QAbstractSlider::SliderSingleStepSub:
        value -= slider->singleStep();
        emitSignal = true;
        break;
    case QAbstractSlider::SliderSingleStepAdd:
        value += slider->singleStep();
        emitSignal = true;
        break;
    case QAbstractSlider::SliderPageStepAdd:
        value += slider->pageStep();
        emitSignal = true;
        break;
    case QAbstractSlider::SliderToMinimum:
        value = slider->minimum();
        emitSignal = true;
        break;
    case QAbstractSlider::SliderToMaximum:
        value += slider->maximum();
        emitSignal = true;
        break;
    case QAbstractSlider::SliderMove:
        if (!slider->isSliderDown()) {
            // Mouse wheel
            emitSignal = true;
        }
        break;

    }

    if (emitSignal) {
        emit positionChanged ((qint64) (value * 1000));
    }
}

void VideoControlPanel::on_sliderPositionMoved(int value)
{
    emit positionChanged ((qint64) (value * 1000));
}

void VideoControlPanel::on_playClicked(bool checked)
{
    if (checked) {
        emit playClicked();
    }
    else {
        emit pauseClicked();
    }
}

void VideoControlPanel::on_play()
{
    ((QPushButton *) _buttonPlay->widget())->setChecked(true);
}

void VideoControlPanel::on_pause()
{
    ((QPushButton *) _buttonPlay->widget())->setChecked(false);
}

void VideoControlPanel::on_stop()
{
    ((QPushButton *) _buttonPlay->widget())->setChecked(false);
}

void VideoControlPanel::setDuration(qint64 value)
{
    QSlider *position;

    position = (QSlider *) _sliderPosition->widget();
    position->setMaximum(value / 1000);

    _duration = QTime(0, 0, 0, 0).addMSecs(value);
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
