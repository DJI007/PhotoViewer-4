#include "mapview.h"

MapView::MapView() :
    QQuickView()
{
    setSource(QUrl("qrc:///qml/MapViewer.qml"));
    _mapItem = rootObject()->findChild<QQuickItem *>("map");
}

MapView::~MapView()
{
}

void MapView::setPosition(double latitude, double longitude, double altitude)
{
    Q_UNUSED(altitude);

    QMetaObject::invokeMethod(_mapItem,
                              "setPosition",
                              Q_ARG(QVariant, latitude),
                              Q_ARG(QVariant, longitude));
}
