import QtQuick 2.12

import Neo.Quick 1.0

ListView {
    id: control

    property int itemWidth: 100
    property int itemHeight: 30
    orientation: ListView.Horizontal

    //    implicitWidth: 200
    //    implicitHeight: 300
    interactive: false
    moveDisplaced: Transition {
        NumberAnimation { properties: "x,y"; duration: 300 }
    }
    property int dragedIndex: -1
    property int dragedMoveX: 0


    delegate: Item {
        id: delegateItem
        width: itemWidth
        height: itemHeight
        z: dragArea.drag.active ? Infinity : 1

        Rectangle {
            id: delegateRect
            width: parent.width
            height: parent.height
            color: modelColor

            onXChanged: {
                if(dragedIndex != index || delegateRect.x > delegateItem.width) {
                    return
                }
                var toRight = delegateRect.x - dragedMoveX > 0
                var toLeft = !toRight
                var toIndex = toRight ? index + 1 : index - 1
                var fromIndex = index
                if(fromIndex === toIndex || toIndex < 0 || toIndex >= control.count) {
                    return;
                }
                //当按着不放连续移动时，需减去模型数据移动时的位置（width/2)
                var thresold = delegateRect.width / 2
                var toThresold = Math.abs(delegateRect.x) > thresold
                if(!toThresold) {
                    return
                }
                control.model.move(fromIndex,toIndex,1)
                dragedIndex = toIndex
                if(toRight) {
                    dragedMoveX = delegateRect.x - delegateItem.width
                } else {
                    dragedMoveX = delegateRect.width + delegateRect.x
                }
            }

            Rectangle {
                width: 2
                color: "#ffffff"
                height: parent.height
                anchors.centerIn: parent
            }

            Drag.dragType: Drag.Internal
            Text {
                text: model.content
            }

            MouseArea {
                id: dragArea
                anchors.fill: parent
                drag {
                    target: delegateRect
                    axis: Drag.XAxis
                }
                onReleased: {
                    delegateRect.x = 0
                    dragedMoveX = 0
                }
                onPressed: {
                    dragedIndex = index
                }
            }
        }
    }
}


//            MouseArea {
//                anchors.fill: parent
//                cursorShape: pressed ? Qt.PointingHandCursor : Qt.ArrowCursor
//                onPositionChanged: {
//                    var mouseIndex = control.indexAt(mouseX + delegateRect.x, mouseY + delegateRect.y)
//                    if ((mouseIndex < 0) || (mouseIndex > count))
//                        return;
//                    if(mouseIndex !== index) {
//                        control.model.move(index,mouseIndex,1)
//                    }
//                }
//            }
