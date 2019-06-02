import QtQuick 2.12

import Neo.Quick 1.0

ListView {
    id: control

    implicitWidth: 200
    implicitHeight: 300

    property int itemWidth: parent.width
    property int itemHeight: 30

    delegate: Item {
        width: itemWidth
        height: itemHeight
        Text {
            text: modelData
        }
        MovableArea {
            anchors.fill: parent
            cursorShape: Qt.DragMoveCursor
            target: parent
            onPositionChanged: {
                print(deltaX,deltaY)
            }
        }
    }
}
