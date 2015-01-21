#ifndef PICTUREVIEW_H
#define PICTUREVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QImage>
#include <QAnimationGroup>
#include <QGraphicsItem>

#include "exifmetadata.h"
#include "clickabletextitem.h"
#include "pictureanimation.h"
#include "animateditem.h"

#include "pictureviewitemcontainer.h"

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

    void showFileInfo ();
    void showGeoInfo ();
    void showRating ();

    void hideFileInfo ();
    void hideGeoInfo ();
    void hideRating ();

    void setShowTime (int time);

    void rotatePictureLeft ();
    void rotatePictureRight ();

signals:
    void mouseDoubleClick (QMouseEvent *event);
    void mouseMove (QMouseEvent *event);
    void requestMapWindow (double latitude, double longitude, double altitude);

    void beginItemAnimationIn ();
    void endItemAnimationIn ();
    void beginItemAnimationOut ();
    void endItemAnimationOut ();
    void showTimeEnded ();

public slots:
    void loadPicture (QString fileName);
    void showPicture (PictureAnimationType animType /* = PictureAnimationType::None */);
    void setPictureRating (int rating);
    void on_finishPrevItemAnimation ();
    void on_finishCurrentItemAnimation ();
    void on_itemRequestMapWindow (double latitude, double longitude, double altitude);
    void on_itemLoaded ();
    void resize ();

    void on_showTimeEnded ();

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *event);

private:
    const int ANIMATION_DURATION_MILLISECONDS = 3000;

    QGraphicsScene *_pictureScene;
    PictureViewItemContainer *_currentItem;
    PictureViewItemContainer *_prevItem;
    QAnimationGroup *_currentAnimation;
    PictureAnimationType _currentAnimationType;
    bool _infoVisible;
    int _showTime;

    QList<AbstractPictureAnimation *> _animations;
};

#endif // PICTUREVIEW_H
