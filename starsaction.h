#ifndef STARSACTION_H
#define STARSACTION_H

#include <QWidgetAction>
#include "starlabel.h"

class StarsAction : public QWidgetAction
{
    Q_OBJECT
public:
    explicit StarsAction(QWidget *parent = 0);


protected:
    virtual QWidget *createStars (QWidget *parent);

signals:

public slots:
   void on_star_mouseEnter (StarLabel *sender);
   void on_star_mouseLeave (StarLabel *sender);

private:
   StarLabel *_star1;
   StarLabel *_star2;
   StarLabel *_star3;
   StarLabel *_star4;
   StarLabel *_star5;

   void connectSignals (StarLabel *lbl);
};

#endif // STARSACTION_H
