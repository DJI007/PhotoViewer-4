#ifndef VIDEOITEM_H
#define VIDEOITEM_H

#include <QGraphicsVideoItem>
#include <QMediaPlayer>
#include <QGraphicsRectItem>
#include <QGraphicsProxyWidget>

#include "pictureviewitem.h"
#include "exifmetadata.h"

class VideoItem : public QGraphicsVideoItem,
        public PictureViewItem
{
    Q_OBJECT

    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation)
    Q_PROPERTY(qreal scale READ scale WRITE setScale)

public:
    explicit VideoItem(QString fileName, QObject *parent = 0);
    ~VideoItem();

    void load ();
    void resize ();
    QDateTime getDate ();

    AbstractMetadata *metadata ();

signals:
    void itemLoaded ();

public slots:
    void on_mediaStatusChanged(QMediaPlayer::MediaStatus status);
    void on_videoAvailableChanged(bool available);
    void on_stateChanged(QMediaPlayer::State state);
    void on_nativeSizeChanged(const QSizeF & size);

    void on_beginItemAnimation();
    void on_endItemAnimation();

    void on_positionChanged (qint64 value);

private:
    QMediaPlayer *_player;
    QString _fileName;

    QGraphicsRectItem *_panel;
    QGraphicsProxyWidget *_sliderPosition;
    QGraphicsProxyWidget *_buttonPlay;


    ExifMetadata *_videoData;

    void createControls ();

};

#endif // VIDEOITEM_H
