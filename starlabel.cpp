#include "starlabel.h"

#include <QEvent>
#include <QDebug>
#include <QToolBar>


StarLabel::StarLabel(QWidget *parent) :
    QLabel(parent)
{
    qDebug() << "Creating label";

    QPixmap *tmp;

    tmp = new QPixmap(":/images/images/star-on.png");
    _pixmapStarOn = tmp->scaledToHeight(this->height() - 8, Qt::SmoothTransformation);

    delete tmp;

    tmp = new QPixmap(":/images/images/star-off.png");
    _pixmapStarOff = tmp->scaledToHeight(this->height() - 8, Qt::SmoothTransformation);

    delete tmp;

    this->installEventFilter(this);
    this->setPixmap(_pixmapStarOff);
}

StarLabel::~StarLabel()
{
}

bool StarLabel::eventFilter(QObject *object, QEvent *event)
{
    Q_UNUSED (object);

    if (event->type() == QEvent::Enter) {
        emit mouseEnter(this);
    }
    else if (event->type() == QEvent::Leave) {
        emit mouseLeave(this);
    }

    return false;
}

void StarLabel::setOn()
{
    this->setPixmap(_pixmapStarOn);
}

void StarLabel::setOff()
{
    this->setPixmap(_pixmapStarOff);
}
