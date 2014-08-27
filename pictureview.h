#ifndef PICTUREVIEW_H
#define PICTUREVIEW_H

#include <QGraphicsView>
#include <QWidget>

class PictureView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit PictureView(QWidget *parent = 0);

signals:
    void mouseDoubleClick (QMouseEvent *);

public slots:

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *);
};

#endif // PICTUREVIEW_H
