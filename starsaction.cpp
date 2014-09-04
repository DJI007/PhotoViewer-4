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
    this->setDefaultWidget(this->createStars(parent));
}


QWidget *StarsAction::createStars (QWidget *parent)
{
    QWidget *tmp;
    QHBoxLayout *layout;

    tmp = new QWidget (parent);
    layout = new QHBoxLayout (tmp);
    layout->setSpacing(0);

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

    connectSignals (_star1);
    connectSignals (_star2);
    connectSignals (_star3);
    connectSignals (_star4);
    connectSignals (_star5);

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
    if (sender == _star1) {
        qDebug () << "1 Star for the image!!";
    }
    else if (sender == _star2) {
        qDebug () << "2 Stars for the image!!";
    }
    else if (sender == _star3) {
        qDebug () << "3 Stars for the image!!";
    }
    else if (sender == _star4) {
        qDebug () << "4 Stars for the image!!";
    }
    else if (sender == _star5) {
        qDebug () << "5 Stars for the image!!";
    }
}
