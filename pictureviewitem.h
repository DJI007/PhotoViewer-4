#ifndef PICTUREVIEWITEM_H
#define PICTUREVIEWITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QDateTime>

#include "animateditem.h"
#include "abstractmetadata.h"

class PictureViewItem : public AnimatedItem
{
public:
    explicit PictureViewItem(QObject *parent = 0);

    // operator QGraphicsItem* () { return dynamic_cast<QGraphicsItem *> (this); }
    // operator QObject* () { return dynamic_cast<QObject *> (this); }

    virtual void load () = 0;
    virtual void resize () = 0;
    virtual QDateTime getDate () = 0;

    virtual AbstractMetadata *metadata () = 0;

signals:
    void itemLoaded ();

public slots:
    virtual void on_beginItemAnimation () {}
    virtual void on_endItemAnimation () {}

};

#endif // PICTUREVIEWITEM_H
