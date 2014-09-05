#ifndef PICTUREVIEW_H
#define PICTUREVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QImage>
#include <QAbstractAnimation>
#include <QGraphicsItem>

#include "exifmetadata.h"
#include "animateditempicture.h"
#include "animateditemtext.h"

class PictureView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit PictureView(QWidget *parent = 0);
    ~PictureView ();

    void setPicture (QString);
    bool hasPicture ();
    void showPictureWithAnimation ();

signals:
    void mouseDoubleClick (QMouseEvent *);

public slots:
    void showPicture ();
    void setPictureRating (int rating);
    void on_finish_outAnimation ();

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *);

private:
    QString _fileName;
    QImage *_picture;
    QGraphicsScene *_pictureScene;
    ExifMetadata _pictureData;

    QPixmap correctOrientationPicture();
    QPixmap scaledImage(QPixmap src);

    AnimatedItemPicture *createPicture ();
    AnimatedItemText *createInfo ();
    QGraphicsItemGroup *createRating ();

    AnimatedItemPicture *createStar (bool, int left, int top);

    QAbstractAnimation *createAnimationIn (QGraphicsItemGroup *);
    QAbstractAnimation *createAnimationOut (QGraphicsItemGroup *);
};

#endif // PICTUREVIEW_H
