#ifndef PICTUREVIEW_H
#define PICTUREVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QImage>

#include "exifmetadata.h"

class PictureView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit PictureView(QWidget *parent = 0);
    ~PictureView ();

    void setPicture (QString);
    bool hasPicture ();

signals:
    void mouseDoubleClick (QMouseEvent *);

public slots:
    void showPicture ();

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *);

private:
    QString _fileName;
    QImage *_picture;
    QGraphicsScene *_pictureScene;
    ExifMetadata _pictureData;

    void addPicture ();
    void addInfo ();
    void addRating ();
    void addStar (bool, int left, int top);
};

#endif // PICTUREVIEW_H
