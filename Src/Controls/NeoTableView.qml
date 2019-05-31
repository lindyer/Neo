import QtQuick 2.12
import QtQuick.Controls 2.4

import Neo.Quick 1.0

Item {
    id: control

    //export alias property
    property alias model: tableView.model
    property alias tableView: tableView

    //header properties
    property var headerList: []  // it need two field: Title and Width
    property color headerSplitColor: "#D8D8D8"

    //row properties
    property color normalColor: "transparent"
    property color hoverColor: "#EEEEEE"
    property color selectColor: "#CCCCCC"
    property int rowHeight: rh(30)
    property int currentHoverRow: -1
    property int currentSelectRow: -1

    //whole table properties
    property bool multiSelectMode: false
    property int contentWidth: 0
    property var selectedRows: []
    property int titleBarHeight: rh(30)
    property var multiSelectCond: function(row,cellValue) {
        return selectedRows.indexOf(row) >= 0
    }

    //signals
    signal rowDoubleClicked(int row)

    onHeaderListChanged: {
        contentWidth = 0
        for(var i in headerList) {
            var item = headerList[i]
            if(!item.hasOwnProperty("Visible")) {
                item["Visible"] = true
            } else if(!item["Visible"]) {
                contentWidth += headerList[i].Width
            }
            var headerItemActionObject = _headerItemAction.createObject(null,{text:item["Title"],checked:true})
            _headerMenu.addAction(headerItemActionObject)
            headerModel.append(item)
        }
    }

    Component {
        id: _headerItemAction
        Action {
            checkable: true
            onToggled: {
                print("##",text,source)
            }
        }
    }

    NeoMenu {
        id: _headerMenu
        title: "显示的列"
    }

    ListView {
        id: headerListView
        width: parent.width
        height: titleBarHeight
        orientation: ListView.Horizontal
        contentX: tableView.contentX
        model: ListModel {
            id: headerModel
        }
        interactive: false
        clip: true

        delegate: Rectangle {
            id: headerItemDelegate
            width: Visible ? Width : 0
            height: titleBarHeight
            color: index % 2 ?  "#FFFFFF": "#F0F0F0"
            visible: Visible
            z: index

            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.RightButton
                onClicked: {
                     if(mouse.button === Qt.RightButton) {
                        _headerMenu.popup()
                    }
                }
            }

            Item {
                width: rw(5)
                height: parent.height
                anchors {
                    right: parent.right
                }

                Rectangle {
                    width: 1
                    height: parent.height
                    anchors.right: parent.right
                    color: "#D8D8D8"
                }

                MovableArea {
                    id: headerItemMouseArea
                    anchors.fill: parent
                    hoverEnable: true
                    cursorShape: Qt.SizeHorCursor
                    onPositionChanged: {
                        if(Width <= rw(40) && deltaX < 0) {
                            return;
                        }
                        Width += deltaX;
                        var delta = Width - headerList[index].Width
                        headerList[index].Width = Width
                        contentWidth += delta
                        tableView.forceLayout()
                    }
                }
            }

            Text {
                text: Title
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: rfs(12)
                elide: Text.ElideRight
            }
        }
    }

    TableView {
        id: tableView
        contentX: headerListView.contentX
        anchors.fill: parent
        columnWidthProvider: function columnWidth(column) {
            return headerList[column].Width;
        }
        contentWidth: control.contentWidth
        anchors.topMargin: titleBarHeight
        flickableDirection: TableView.VerticalFlick
        clip: true
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
                top: tableView.top
                bottom: tableView.bottom
                right: tableView.right
                rightMargin: rw(-14)
            }
        }

        delegate: Rectangle {
            implicitHeight: rowHeight
            color: {
                if(multiSelectMode) {
                    return multiSelectCond(row,Role) ? selectColor : currentHoverRow == row ? hoverColor : normalColor
                } else {
                    return currentSelectRow == row ? selectColor : currentHoverRow == row ? hoverColor : normalColor
                }
            }
            Text  {
                text: Role
                anchors.centerIn: parent
            }
            Rectangle {
                width: parent.width
                anchors.bottom: parent.bottom
                height: 1
                color: "#F0F0F0"
            }
            Rectangle {
                anchors.right: parent.right
                width: 1
                height: parent.height
                color: "#F0F0F0"
            }

            MouseArea {
                id: _tableItemMouseArea
                anchors.fill: parent
                hoverEnabled: true
                onContainsMouseChanged: {
                    if(containsMouse) {
                        currentHoverRow = row
                    } else {
                        currentHoverRow = -1
                    }
                }
                onDoubleClicked: control.rowDoubleClicked(row)
            }
        }
    }

}

