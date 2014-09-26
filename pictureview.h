#ifndef PICTUREVIEW_H
#define PICTUREVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QImage>
#include <QAnimationGroup>
#include <QGraphicsItem>

#include "exifmetadata.h"
#include "clickableitemtext.h"
#include "pictureanimation.h"
#include "animateditem.h"

class PictureView : public QGraphicsView
{
    Q_OBJECT
public:
    enum PictureAnimationType {
        None,
        LeftToRight,
        RightToLeft,
        Random
    };

    explicit PictureView(QWidget *parent = 0);
    ~PictureView ();

    bool hasPicture ();
    void setNormalBackground ();
    void setFullScreenBackground ();
    void cleanPicture ();

    double pictureLatitude ();
    double pictureLongitude ();

    void setInfoVisible (bool visible);

signals:
    void mouseDoubleClick (QMouseEvent *event);
    void mouseMove (QMouseEvent *event);
    void requestMapWindow (double latitude, double longitude, double altitude);


public slots:
    void loadPicture (QString fileName);
    void showPicture (PictureAnimationType animType /* = PictureAnimationType::None */);
    void setPictureRating (int rating);
    void on_finishPrevItemAnimation ();
    void on_finishCurrentItemAnimation ();
    void resize ();
    void on_pictureRequestMapWindow (double latitude, double longitude, double altitude);

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *event);

private:
    const int ANIMATION_DURATION_MILLISECONDS = 3000;

    QGraphicsScene *_pictureScene;
    AnimatedItem *_currentItem;
    AnimatedItem *_prevItem;
    QAnimationGroup *_currentAnimation;
    bool _infoVisible;

    QList<AbstractPictureAnimation *> _animations;
};

#endif // PICTUREVIEW_H
