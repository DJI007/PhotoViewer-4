#ifndef PICTUREVIEW_H
#define PICTUREVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QImage>
#include <QAnimationGroup>
#include <QGraphicsItem>

#include "exifmetadata.h"
#include "animateditempicture.h"
#include "animateditemtext.h"
#include "pictureanimation.h"

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

signals:
    void mouseDoubleClick ();
    void mouseMove ();
    void requestMapWindow (double latitude, double longitude, double altitude);


public slots:
    void loadPicture (QString fileName);
    void showPicture (PictureAnimationType animType /* = PictureAnimationType::None */);
    void setPictureRating (int rating);
    void on_finishPrevPictureAnimation ();
    void resize ();
    void on_pictureRequestMapWindow (double latitude, double longitude, double altitude);

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *event);

private:
    const int ANIMATION_DURATION_MILLISECONDS = 3000;

    QGraphicsScene *_pictureScene;
    AnimatedItemPicture *_currentPicture;
    AnimatedItemPicture *_prevPicture;
    QAnimationGroup *_currentAnimation;

    QList<AbstractPictureAnimation *> _animations;
};

#endif // PICTUREVIEW_H
