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

    QGraphicsScene *getScene ();


signals:
    void mouseDoubleClick (QMouseEvent *);
    void mouseMove (QMouseEvent *);

public slots:
    void loadPicture (QString fileName);
    void showPicture (PictureAnimationType animType /* = PictureAnimationType::None */);
    void setPictureRating (int rating);
    void on_finishPrevPictureAnimation ();
    void resize ();

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *event);

private:
    QGraphicsScene *_pictureScene;
    AnimatedItemPicture *_currentPicture;
    AnimatedItemPicture *_prevPicture;
    QAnimationGroup *_currentAnimation;
    QAbstractAnimation *createAnimationIn (QGraphicsItemGroup *);
    QAbstractAnimation *createAnimationOut (QGraphicsItemGroup *);
};

#endif // PICTUREVIEW_H
