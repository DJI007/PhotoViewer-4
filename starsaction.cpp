#include "starsaction.h"

#include <QPushButton>
#include <QDebug>
#include <QPixmap>
#include <QHBoxLayout>
#include <QSpinBox>

#include "starlabel.h"


StarsAction::StarsAction(QWidget *parent) :
    QWidgetAction(parent)
{
}


QWidget *StarsAction::createWidget (QWidget *parent)
{
    QWidget *tmp;
    QHBoxLayout *layout;

    tmp = new QWidget (parent);
    layout = new QHBoxLayout (tmp);
    layout->setSpacing(0);

    _star0 = new StarLabel (tmp);
    _star0->setCancel();
    _star0->show();
    _star1 = new StarLabel (tmp);
    _star1->show();
    _star2 = new StarLabel (tmp);
    _star2->show();
    _star3 = new StarLabel (tmp);
    _star3->show();
    _star4 = new StarLabel (tmp);
    _star4->show();
    _star5 = new StarLabel (tmp);
    _star5->show();

    connectSignals (_star0);
    connectSignals (_star1);
    connectSignals (_star2);
    connectSignals (_star3);
    connectSignals (_star4);
    connectSignals (_star5);

    layout->addWidget(_star0);
    layout->addWidget(_star1);
    layout->addWidget(_star2);
    layout->addWidget(_star3);
    layout->addWidget(_star4);
    layout->addWidget(_star5);

    tmp->setLayout(layout);

    return tmp;
}

void StarsAction::connectSignals (StarLabel *lbl)
{
    QObject::connect(lbl,
                     SIGNAL(mouseEnter(StarLabel*)),
                     this,
                     SLOT(on_star_mouseEnter(StarLabel *)));

    QObject::connect(lbl,
                     SIGNAL(mouseLeave(StarLabel*)),
                     this,
                     SLOT(on_star_mouseLeave(StarLabel *)));

    QObject::connect(lbl,
                     SIGNAL(mouseClick(StarLabel*)),
                     this,
                     SLOT(on_star_mouseClick(StarLabel *)));
}

void StarsAction::on_star_mouseEnter (StarLabel *sender)
{
    if (sender == _star1) {
        _star1->setOn();
        _star2->setOff();
        _star3->setOff();
        _star4->setOff();
        _star5->setOff();
    }
    else if (sender == _star2) {
        _star1->setOn();
        _star2->setOn();
        _star3->setOff();
        _star4->setOff();
        _star5->setOff();
    }
    else if (sender == _star3) {
        _star1->setOn();
        _star2->setOn();
        _star3->setOn();
        _star4->setOff();
        _star5->setOff();
    }
    else if (sender == _star4) {
        _star1->setOn();
        _star2->setOn();
        _star3->setOn();
        _star4->setOn();
        _star5->setOff();
    }
    else if (sender == _star5) {
        _star1->setOn();
        _star2->setOn();
        _star3->setOn();
        _star4->setOn();
        _star5->setOn();
    }
}

void StarsAction::on_star_mouseLeave (StarLabel *sender)
{
    Q_UNUSED(sender);

    _star1->setOff();
    _star2->setOff();
    _star3->setOff();
    _star4->setOff();
    _star5->setOff();
}

void StarsAction::on_star_mouseClick(StarLabel *sender)
{
    if (sender == _star0) {
        emit setRating(0);
    }
    else if (sender == _star1) {
        emit setRating(1);
    }
    else if (sender == _star2) {
        emit setRating(2);
    }
    else if (sender == _star3) {
        emit setRating(3);
    }
    else if (sender == _star4) {
        emit setRating(4);
    }
    else if (sender == _star5) {
        emit setRating(5);
    }
}
