import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.12
import "../Effects"

Menu {
    id: control
    property int itemHeight: rh(30)

    topPadding: 2
    bottomPadding: 2
    height: rh(300)

    contentItem: ColumnLayout {
        height: control.height
        Text {
            text: control.title
            Layout.preferredHeight: rh(24)
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: rfs(14)
            leftPadding: rw(8)
        }

        ListView {
            clip: true
            width: control.width
            Layout.fillHeight: true
            model: control.contentModel
        }
    }

    delegate: MenuItem {
        id: menuItem
        implicitWidth: 200
        implicitHeight: itemHeight

        arrow: Canvas {
            x: parent.width - width
            implicitWidth: 40
            implicitHeight: itemHeight
            visible: menuItem.subMenu
            onPaint: {
                var ctx = getContext("2d")
                ctx.fillStyle = menuItem.highlighted ? "#ffffff" : "#21be2b"
                ctx.moveTo(15, 15)
                ctx.lineTo(width - 15, height / 2)
                ctx.lineTo(15, height - 15)
                ctx.closePath()
                ctx.fill()
            }
        }

        indicator: Item {
            implicitWidth: 40
            implicitHeight: itemHeight
            Rectangle {
                width: 26
                height: 26
                anchors.centerIn: parent
                visible: menuItem.checkable
                border.color: "#21be2b"
                radius: 3
                Rectangle {
                    width: 14
                    height: 14
                    anchors.centerIn: parent
                    visible: menuItem.checked
                    color: "#21be2b"
                    radius: 2
                }
            }
        }

        contentItem: Text {
            leftPadding: menuItem.indicator.width
            rightPadding: menuItem.arrow.width
            text: menuItem.text
            font: menuItem.font
            opacity: enabled ? 1.0 : 0.3
            color: menuItem.highlighted ? "#ffffff" : "#21be2b"
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        background: Rectangle {
            implicitWidth: 200
            implicitHeight: itemHeight
            opacity: enabled ? 1 : 0.3
            color: menuItem.highlighted ? "#21be2b" : "transparent"
        }
    }


    background: NeoDropShadow {
        implicitWidth: 200
        implicitHeight: itemHeight
        contentItem: Rectangle {
            color: "#FFF"
            border.color: "#E0E0E0"
            radius: 2
        }
    }
}
