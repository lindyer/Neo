import QtQuick 2.12
import QtGraphicalEffects 1.0

Item {

    property alias contentItem: contentLoader.sourceComponent

    property alias dropShadow: dropShadow

    anchors.fill: parent

    Loader {
        id: contentLoader
        anchors.fill: parent
    }

    DropShadow {
        id: dropShadow
        anchors.fill: contentLoader
        samples: 20
        color: "#66000000"
        source: contentLoader
    }
}
