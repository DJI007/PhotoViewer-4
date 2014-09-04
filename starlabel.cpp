#include "starlabel.h"

#include <QEvent>
#include <QDebug>
#include <QToolBar>
#include <QMouseEvent>


StarLabel::StarLabel(QWidget *parent) :
    QLabel(parent)
{
    QPixmap *tmp;

    tmp = new QPixmap(":/images/images/star-on.png");
    _pixmapStarOn = tmp->scaledToHeight(this->height() - 10, Qt::SmoothTransformation);

    delete tmp;

    tmp = new QPixmap(":/images/images/star-off.png");
    _pixmapStarOff = tmp->scaledToHeight(this->height() - 10, Qt::SmoothTransformation);

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

        return true;
    }
    else if (event->type() == QEvent::Leave) {
        emit mouseLeave(this);

        return true;
    }
    else if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mEvent;

        mEvent = static_cast<QMouseEvent *> (event);
        if (mEvent->button() == Qt::LeftButton) {
            emit mouseClick(this);

            return true;
        }
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
