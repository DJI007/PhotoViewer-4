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

    _fileName = fileName;
    _picture = new QImage (fileName);
    _pictureData.loadData(fileName);
}


bool PictureView::hasPicture()
{
    return (_picture != NULL) && (!_picture->isNull());
}

void PictureView::showPicture()
{
    _pictureScene->clear();

    addPicture ();
    addInfo ();
    addRating ();
}

void PictureView::addPicture()
{
    QGraphicsItem *item;
    qreal scale;
    item = new QGraphicsPixmapItem(QPixmap::fromImage(*_picture));

    scale =  ((qreal) (this->width() - 4) / (qreal) _picture->width());
    if ((_picture->height() * scale) > this->height()) {
        scale =  ((qreal) (this->height() - 4) / (qreal) _picture->height());
    }

    int newWidth;
    int newHeight;
    QRect rect;

    item->setScale(scale);

    newWidth = _picture->width() * scale;
    newHeight = _picture->height() * scale;

    rect.adjust(_pictureScene->sceneRect().left(),
                _pictureScene->sceneRect().top(),
                newWidth,
                newHeight);

    _pictureScene->setSceneRect(rect);

    _pictureScene->addItem(item);
}

void PictureView::addInfo()
{
    QGraphicsTextItem *item;
    QString msg;

    msg = "<span style=\"background-color: black; color: white; margin:5px 5px 5px 5px\">";
    msg += _fileName;
    msg += "<br />";
    msg += _pictureData.getPictureDate().toString(Qt::SystemLocaleLongDate);
    msg += "</span>";

    QRectF rect;

    rect = _pictureScene->sceneRect();

    item = new QGraphicsTextItem();
    item->setPos(rect.left(), rect.bottom() - 40);
    item->setHtml(msg);

    _pictureScene->addItem(item);
}

void PictureView::addRating()
{
    int rating;
    int left;
    int top;
    QRectF rect;

    rect = _pictureScene->sceneRect();
    rating = _pictureData.getRating();
    left = rect.right() - 30;
    top = rect.top() + 5;

    for (int i = 0; i < rating; i++) {
        addStar (true, left, top);
        left -= 20;
    }

    for (int i = rating; i < 5; i++) {
        addStar (false, left, top);
        left -= 20;
    }
}

void PictureView::addStar (bool isOn, int left, int top)
{
    QGraphicsItem *item;
    QPixmap *star;

    if (isOn) {
        star = new QPixmap(":/images/images/star-on.png");
    }
    else {
        star = new QPixmap(":/images/images/star-off.png");
    }

    item = new QGraphicsPixmapItem(star->scaledToHeight(20, Qt::SmoothTransformation));
    item->setPos(left, top);

    _pictureScene->addItem(item);

    delete star;
}

