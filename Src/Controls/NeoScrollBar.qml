import QtQuick 2.12
import QtQuick.Controls 2.12


/*
  assign scrollbar parent
        ScrollBar.horizontal: NeoScrollBar {
            policy: ScrollBar.AlwaysOn
            orientation: Qt.Horizontal
            parent: tableView.parent
            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom
                bottomMargin: rw(-14)
            }
        }
        ScrollBar.vertical: NeoScrollBar {
            policy: ScrollBar.AlwaysOn
            parent: tableView.parent
            anchors {
                top: parent.top
                bottom: parent.bottom
                right: parent.right
                rightMargin: rw(-14)
            }
        }
*/

ScrollBar {
    id: control

    property int radius: orientation == Qt.Vertical ? width / 2 : height / 2
    property color color: "#eeeeee"
    property int baseSize: rw(12)

    width: orientation == Qt.Vertical ? baseSize : parent.width
    height: orientation == Qt.Vertical ? parent.height : baseSize
    orientation: Qt.Vertical
    minimumSize: 0.2

    background: Rectangle {
        width: control.width
        height: control.height
        color: control.color
        radius: control.radius
    }
}
