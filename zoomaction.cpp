
#include "zoomaction.h"

#include <QHBoxLayout>

ZoomAction::ZoomAction(QWidget *parent) :
    QWidgetAction(parent)
{
}

QWidget *ZoomAction::createWidget(QWidget *parent)
{
    QWidget *tmp;
    QHBoxLayout *layout;

    tmp = new QWidget (parent);
    tmp->setContentsMargins(0, 0, 0, 0);
    tmp->setFixedWidth(130);

    layout = new QHBoxLayout (tmp);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    _zoomOut = new QToolButton (tmp);
    _zoomOut->setAutoRaise(true);
    _zoomOut->setIcon(QIcon(":/images/images/zoom-out.svg"));
    _zoomOut->setEnabled(false);

    _slider = new QSlider (Qt::Horizontal, tmp);
    _slider->setSingleStep(20);
    _slider->setPageStep(50);
    _slider->setMinimum(0);
    _slider->setMaximum(200);
    _slider->setMinimumWidth(50);
    _slider->setMaximumWidth(50);
    _slider->setFixedWidth(50);

    _zoomIn = new QToolButton (tmp);
    _zoomIn->setAutoRaise(true);
    _zoomIn->setIcon(QIcon(":/images/images/zoom-in.svg"));

    _percent = new QLabel (tmp);
    _percent->setText("0%");
    _percent->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    connect (_zoomIn, SIGNAL(clicked()),
             this, SLOT(onZoomInClicked()));
    connect (_zoomOut, SIGNAL(clicked()),
             this, SLOT(onZoomOutClicked()));
    connect (_slider, SIGNAL(valueChanged(int)),
             this, SLOT(onZoomValueChanged(int)));

    layout->addWidget(_zoomOut);
    layout->addWidget(_slider);
    layout->addWidget(_zoomIn);
    layout->addWidget(_percent);

    tmp->setLayout(layout);

    return tmp;
}

void ZoomAction::onZoomInClicked()
{
    _slider->triggerAction(QAbstractSlider::SliderSingleStepAdd);
}

void ZoomAction::onZoomOutClicked()
{
    _slider->triggerAction(QAbstractSlider::SliderSingleStepSub);
}

void ZoomAction::onZoomValueChanged(int value)
{
    _zoomOut->setEnabled(value > 0);
    _zoomIn->setEnabled(value < (_slider->maximum()));
    _percent->setText(QString ("%1\%").arg(value));

    emit zoomChanged(value);
}

qreal ZoomAction::value()
{
    return _slider->value();
}

void ZoomAction::setValue(qreal value)
{
    _slider->setValue(value);
}

