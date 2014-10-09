#ifndef VIDEOCONTROLPANEL_H
#define VIDEOCONTROLPANEL_H

#include <QGraphicsRectItem>
#include <QGraphicsProxyWidget>

class VideoControlPanel : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    explicit VideoControlPanel(QGraphicsItem *parent = 0);

signals:
    void rewindClicked ();
    void stopClicked ();
    void playToggled (bool);
    void fastForwardClicked ();

    void positionChanged (qint64);
    void volumeChanged (int);

public slots:
    void on_positionChanged (qint64 value);

    void setMaximum (int value);
    void setMinimum (int value);
    void setVolume (int value);

private:
    QGraphicsProxyWidget *_sliderPosition;
    QGraphicsProxyWidget *_buttonRew;
    QGraphicsProxyWidget *_buttonStop;
    QGraphicsProxyWidget *_buttonPlay;
    QGraphicsProxyWidget *_buttonFF;
    QGraphicsProxyWidget *_sliderVolume;

    QGraphicsProxyWidget *addButtonToPanel(QString iconPathOn,
                                           QString iconPathOff,
                                           int posX,
                                           bool checkable);

    QString readCSS (QString path);
};

#endif // VIDEOCONTROLPANEL_H
