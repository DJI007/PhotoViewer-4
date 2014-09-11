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
        RightToLeft
    };

    explicit PictureView(QWidget *parent = 0);
    ~PictureView ();

    void changeSize ();
    bool hasPicture ();
    void showPictureWithAnimation ();

signals:
    void mouseDoubleClick (QMouseEvent *);

public slots:
    void loadPicture (QString fileName);
    void showPicture (PictureAnimationType animType = PictureAnimationType::None);
    void setPictureRating (int rating);
    void on_finish_outAnimation ();
    void resize ();

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *);

private:
    QGraphicsScene *_pictureScene;
    AnimatedItemPicture *_currentPicture;
    AnimatedItemPicture *_prevPicture;
    QAnimationGroup *_currentAnimation;
/*
    QString _fileName;
    QImage *_picture;
    ExifMetadata _pictureData;

    QPixmap correctOrientationPicture();
    QPixmap scaledImage(QPixmap src);

    AnimatedItemPicture *createPicture ();
    AnimatedItemText *createInfo ();
    QGraphicsItemGroup *createRating ();

    AnimatedItemPicture *createStar (bool, int left, int top);
*/

    QAbstractAnimation *createAnimationIn (QGraphicsItemGroup *);
    QAbstractAnimation *createAnimationOut (QGraphicsItemGroup *);
};

#endif // PICTUREVIEW_H
