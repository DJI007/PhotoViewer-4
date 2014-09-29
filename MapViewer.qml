import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import QtPositioning 5.3
import QtLocation 5.2


Rectangle {
    width: 700
    height: 700
    anchors.fill: parent

    property double lastLatitude: 0
    property double lastLongitude: 0

    Plugin {
        id: myPlugin
        name: "osm"
    }

    Map {
        id: mapId
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
            id: mapCircle
            center {
                latitude: -27.5
                longitude: 153.0
            }
            radius: 500.0
            color: 'green'
            border.width: 3
        }

        function setPosition(lat, lon) {
            mapId.center.latitude = lat;
            mapId.center.longitude = lon;

            mapCircle.center.latitude = lat;
            mapCircle.center.longitude = lon;

            lastLatitude = lat;
            lastLongitude = lon;
        }

        Button {
            id: btnCenter
            iconSource: "qrc:///images/images/center.png"
            anchors.right: parent.right
            anchors.bottom: btnPlus.top
            anchors.rightMargin: 10
            anchors.bottomMargin: 3
            style: ButtonStyle {
                background: Rectangle {
                    implicitWidth: 25
                    implicitHeight: 25
                    width: 25
                    height: 25
                    border.width: control.activeFocus ? 2 : 1
                    border.color: "#888"
                    //color: white
                }
            }
            onClicked: {
                if (lastLatitude != 0) {
                    mapId.center.latitude = lastLatitude;
                    mapId.center.longitude = lastLongitude;
                }
            }
        }

        Button {
            id: btnPlus
            text: "+"
            anchors.right: parent.right
            anchors.bottom: btnMinus.top
            anchors.rightMargin: 10
            anchors.bottomMargin: 3
            style: ButtonStyle {
                background: Rectangle {
                    implicitWidth: 25
                    implicitHeight: 25
                    border.width: control.activeFocus ? 2 : 1
                    border.color: "#888"
                    //color: white
                }
            }
            onClicked: {
                if (mapId.zoomLevel < mapId.maximumZoomLevel) {
                    mapId.zoomLevel += 0.3;
                }
            }
        }

        Button {
            id: btnMinus
            text: "-"
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.rightMargin: 10
            anchors.bottomMargin: 10
            style: ButtonStyle {
                background: Rectangle {
                    implicitWidth: 25
                    implicitHeight: 25
                    border.width: control.activeFocus ? 2 : 1
                    border.color: "#888"
                    //color: white
                }
            }

            onClicked: {
                if (mapId.zoomLevel > 0) {
                    mapId.zoomLevel -= 0.3;
                }
            }
        }
    }
}


