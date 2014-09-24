#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QQuickView>
#include <QQuickItem>

namespace Ui {
class MapView;
}

class MapView: public QQuickView
{
    Q_OBJECT

public:
    explicit MapView();
    ~MapView();

    void setPosition (double latitude, double longitude, double altitude);

private:
    QQuickItem *_mapItem;

};

#endif // MAPVIEW_H
