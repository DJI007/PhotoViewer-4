#ifndef VIDEOITEMPHONON_H
#define VIDEOITEMPHONON_H

#include <QGraphicsRectItem>
#include <QPushButton>
#include <QGraphicsObject>
#include <QGraphicsRectItem>
#include <QGraphicsProxyWidget>

#include <phonon/mediaobject.h>
#include <phonon/videowidget.h>
#include <phonon/audiooutput.h>
#include <phonon/MediaController>

#include "abstractmetadata.h"
#include "pictureviewitem.h"
#include "videocontrolpanel.h"

class VideoItemPhonon : public QObject,
        public QGraphicsRectItem,
        public PictureViewItem
{
    Q_OBJECT

    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation)
    Q_PROPERTY(qreal scale READ scale WRITE setScale)

public:
    explicit VideoItemPhonon(QString fileName, QObject *parent = 0);
    ~VideoItemPhonon();

    void load ();
    void resize ();
    QDateTime getDate ();

    AbstractMetadata *metadata ();

    void setShowTime (int time);

    bool rotateLeft();
    bool rotateRight();

    void setRotation (qreal angle);
    qreal rotation ();

    void setScale(qreal scale);
    qreal scale ();

signals:
    void itemLoaded ();
    void playMedia();
    void pauseMedia();
    void stopMedia();
    void volumeChanged(int volume);

    void showTimeEnded ();

    void rotateVideoLeft ();

private slots:
    void on_durationChanged(qint64 value);
    void on_stateChanged (Phonon::State newState, Phonon::State oldState);
    void on_aboutToFinish ();
    void on_availableSubitlesChanged ();

public slots:
    void beginItemAnimationIn();
    void endItemAnimationIn();
    void beginItemAnimationOut();

    void beginRotateLeftAnimation();
    void endRotateLeftAnimation();
    void beginRotateRightAnimation();
    void endRotateRightAnimation();

private:
    Phonon::AudioOutput *_audio;
    Phonon::VideoWidget *_video;
    Phonon::MediaObject *_player;
    Phonon::MediaController *_controller;

    QString _fileName;

    bool _emitShowTimeEnded;

    QGraphicsProxyWidget *_videoItem;
    VideoControlPanel *_panel;

    AbstractMetadata *_videoData;

    void createPanel ();
    void setPanelPosition ();
};


#endif // VIDEOITEMPHONON_H
