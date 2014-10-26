#ifndef VIDEOITEMPHONON_H
#define VIDEOITEMPHONON_H

#include <QGraphicsRectItem>
#include <QPushButton>
#include <QGraphicsProxyWidget>

#include <QMediaPlayer>

#include <phonon/mediaobject.h>
#include <phonon/videowidget.h>
#include <phonon/audiooutput.h>

#include "pictureviewitem.h"
#include "xmpmetadata.h"
#include "videocontrolpanel.h"

class VideoItemPhonon : public QGraphicsProxyWidget,
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

signals:
    void itemLoaded ();
    void playMedia();
    void pauseMedia();
    void stopMedia();
    void volumeChanged(int volume);

    void showTimeEnded ();

private slots:
    void on_durationChanged(qint64 value);
    void on_stateChanged (Phonon::State newState, Phonon::State oldState);
    void on_aboutToFinish ();

public slots:
    void on_beginItemAnimationIn();
    void on_endItemAnimationIn();
    void on_beginItemAnimationOut();

private:
    Phonon::AudioOutput *_audio;
    Phonon::VideoWidget *_video;
    Phonon::MediaObject *_player;

    QString _fileName;

    bool _emitShowTimeEnded;

    VideoControlPanel *_panel;

    XMPMetadata *_videoData;

    void createPanel ();
    void setPanelPosition ();
};


#endif // VIDEOITEMPHONON_H
