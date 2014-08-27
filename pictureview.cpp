#include "pictureview.h"

#include <QMouseEvent>
#include <QDebug>

PictureView::PictureView(QWidget *parent) :
    QGraphicsView(parent)
{

}

void PictureView::mouseDoubleClickEvent (QMouseEvent *event)
{
   if (event->button() == Qt::LeftButton) {
       emit mouseDoubleClick(event);
   }
}
