#ifndef VIDEOCONTROLPANEL_H
#define VIDEOCONTROLPANEL_H

#include <QGraphicsRectItem>
#include <QGraphicsProxyWidget>
#include <QGraphicsTextItem>
#include <QTime>

class VideoControlPanel : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    explicit VideoControlPanel(QGraphicsItem *parent = 0);
    ~VideoControlPanel();

signals:
    void playClicked();
    void pauseClicked ();

    void positionChanged (qint64);
    void volumeChanged (qreal);

private slots:
    void on_sliderPositionActionTriggered(int action);
    void on_sliderVolumeActionTriggered(int action);
    void on_sliderPositionMoved(int value);
    void on_playClicked(bool value);

public slots:
    void play();
    void pause();
    void stop();

    void setPosition (qint64 value);
    void setDuration (qint64 value);
    void setVolume (qreal value);

private slots:
    void on_sliderVolumeMoved (int value);

private:
    QGraphicsProxyWidget *_sliderPosition;
    QGraphicsProxyWidget *_buttonPlay;
    QGraphicsProxyWidget *_sliderVolume;
    QGraphicsTextItem *_textInfo;

    QTime _duration;

    QGraphicsProxyWidget *createButton(QString iconPathOn, QString iconPathOff);
    QGraphicsProxyWidget *createSlider(int x, int y, int width);
    void createIcon (QString file, int x, int y, int width, int height);
    QGraphicsTextItem *createText (int x, int y, QString text);

    QString readCSS (QString path);
};

#endif // VIDEOCONTROLPANEL_H
