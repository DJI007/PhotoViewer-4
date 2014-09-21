import QtQuick 2.0
import QtPositioning 5.2
import QtLocation 5.0

Rectangle {
    width: 600
    height: 600

    Plugin {
        id: myPlugin
        name: "osm"
    }

    Map {
        id: map
        objectName: "map"
        anchors.fill: parent
        plugin: myPlugin;
        center {
            latitude: -27.47
            longitude: 153.025
        }
        zoomLevel: 13
        gesture.enabled: true

        MapCircle {
            center {
                latitude: -27.5
                longitude: 153.0
            }
            radius: 500.0
            color: 'green'
            border.width: 3
        }

        function setPosition(lat, lon) {
            map.center.latitude = lat;
            map.center.longitude = lon;
        }
    }
}

