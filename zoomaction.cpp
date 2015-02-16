
#include "zoomaction.h"

#include <QHBoxLayout>
#include <QToolButton>

ZoomAction::ZoomAction(QObject *parent) :
    QWidgetAction(parent)
{
}

QWidget *ZoomAction::createWidget(QWidget *parent)
{
    QWidget *tmp;
    QHBoxLayout *layout;
    QToolButton *zoomIn;
    QToolButton *zoomOut;

    QPixmap zoomInPX;

    tmp = new QWidget (parent);
    layout = new QHBoxLayout (tmp);
    layout->setSpacing(0);

    zoomInPX = QPixmap(":/images/images/star-on.png").scaledToHeight(tmp->height() - 10, Qt::SmoothTransformation);

    zoomOut = new QToolButton (tmp);
    zoomOut->setAutoRaise(true);
    // zoomOut->setIcon(QIcon(":/images/images/zoom-out.svg"));
    zoomOut->setIcon(QIcon(zoomInPX));
/*
    _slider = new QSlider (Qt::Horizontal, tmp);
    _slider->setMinimum(0);
    _slider->setMaximum(200);
    _slider->setMinimumWidth(50);
    _slider->setMaximumWidth(50);

    zoomIn = new QToolButton (tmp);
    zoomIn->setAutoRaise(true);
    zoomIn->setIcon(QIcon(":/images/images/zoom-in.svg"));
*/
    layout->addWidget(zoomOut);
    // layout->addWidget(_slider);
    // layout->addWidget(zoomIn);

    tmp->setLayout(layout);

    return tmp;
}
