#include "pictureview.h"

#include <QMouseEvent>
#include <QDebug>
#include <QGraphicsPixmapItem>

PictureView::PictureView(QWidget *parent) :
    QGraphicsView(parent)
{
    _pictureScene = new QGraphicsScene(this);
    _pictureScene->setObjectName("gsScene");

    this->setScene(_pictureScene);

    _picture = NULL;
}

PictureView::~PictureView ()
{
    if (_picture != NULL)
        delete _picture;

    delete _pictureScene;
}

void PictureView::mouseDoubleClickEvent (QMouseEvent *event)
{
   if (event->button() == Qt::LeftButton) {
       emit mouseDoubleClick(event);
   }
}

void PictureView::setPicture(QString fileName)
{
    if (_picture != NULL) {
        delete _picture;
    }

    _picture = new QImage (fileName);
    _pictureData.loadData(fileName);
}


bool PictureView::hasPicture()
{
    return (_picture != NULL) && (!_picture->isNull());
}

void PictureView::showPicture()
{
        QGraphicsItem *item;
        qreal scale;
        int newWidth;
        int newHeight;

        item = new QGraphicsPixmapItem(QPixmap::fromImage(*_picture));

        scale =  ((qreal) (this->width() - 4) / (qreal) _picture->width());
        if ((_picture->height() * scale) > this->height()) {
            scale =  ((qreal) (this->height() - 4) / (qreal) _picture->height());
        }
/*
        qDebug() << "Scale: " << QString::number(scale, 'f', 5) << ", _picture->width: " << _picture->width() << ", this->width: " << this->width();
        qDebug() << "Scale: " << QString::number(scale, 'f', 5) << ", _picture->height: " << _picture->height() << ", this->height: " << this->height();
*/
        item->setScale(scale);

        newWidth = _picture->width() * scale;
        newHeight = _picture->height() * scale;

        QRect rect;

        rect.adjust(_pictureScene->sceneRect().left(), _pictureScene->sceneRect().top(), newWidth, newHeight);
        _pictureScene->setSceneRect(rect);
        _pictureScene->clear();
        _pictureScene->addItem(item);

        QGraphicsTextItem * io = new QGraphicsTextItem;
        QString msg;

        msg = "<span style=\"background-color: black; color: white; margin:5px 5px 5px 5px\">";
        msg += _pictureData.getPictureDate().toString(Qt::SystemLocaleLongDate);
        msg += "</span>";

        io->setPos(rect.left(), rect.bottom() - 20);
        io->setHtml(msg);

        // _pictureScene->addRect(io->boundingRect(), QPen(QColor::fromRgb(0, 0, 0)));
        _pictureScene->addItem(io);

}
