#ifndef PICTUREVIEWITEM_H
#define PICTUREVIEWITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QDateTime>
#include <QTimer>

#include "animateditem.h"
#include "abstractmetadata.h"

class PictureViewItem : public AnimatedItem
{
public:
    // operator QGraphicsItem* () { return dynamic_cast<QGraphicsItem *> (this); }
    // operator QObject* () { return dynamic_cast<QObject *> (this); }

    virtual void load () = 0;
    virtual void refresh () { }
    virtual void resize () = 0;
    virtual QDateTime getDate () = 0;

    virtual AbstractMetadata *metadata () = 0;

    virtual void setShowTime (int time) = 0;

    virtual bool rotateLeft () = 0;
    virtual bool rotateRight () = 0;

    virtual qreal zoom () = 0;
    virtual void setZoom (qreal zoomPercent) = 0;

signals:
    void itemLoaded ();
    void showTimeEnded ();

    void zoomChanged (qreal newZoom);

public slots:
    virtual void beginRotateAnimation () {}
    virtual void endRotateAnimation () {}

private:

};

#endif // PICTUREVIEWITEM_H
