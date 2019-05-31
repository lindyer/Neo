import QtQuick 2.12
import QtQuick.Controls 2.4

import Neo.Quick 1.0

Item {
    id: control

    //export alias property
    property alias headerModel: headerListView.model // it need two field: Title and Width
    property alias tableModel: tableView.model
    property alias tableView: tableView

    //header properties
    property color headerSplitColor: "#D8D8D8"

    //row properties
    property color normalColor: "transparent"
    property color hoverColor: "#EEEEEE"
    property color selectColor: "#CCCCCC"
    property int rowHeight: rh(30)
    property int currentHoverRow: -1
    property int currentSelectRow: -1
    property alias itemFont: itemFontHolder.font


    //whole table properties
    property bool selectEnable: false
    property bool multiSelectMode: false
    property int contentWidth: 0
    property var selectedRows: []
    property int titleBarHeight: rh(30)
    property var multiSelectCond: function(row,cellValue) {
        return selectedRows.indexOf(row) >= 0
    }
    /**
     * itemComponentSelector:
     * property var holder: parent
     * parent point to delegate loader which hold itemData,itemRow,itemColumn
     */
    property var itemComponentSelector: function(row,column) {
        return tableItemTextComponent
    }

    //signals
    signal rowDoubleClicked(int row)

    function switchSelect(row) {
        var idx = selectedRows.indexOf(row)
        if(idx < 0) {
            selectedRows.push(row)
        } else {
            selectedRows.splice(idx,1)
        }
    }

    Component {
        id: headerItemActionComponent
        Action {
            checkable: true
            onToggled: {
                print("##",text,source)
            }
        }
    }

    Component {
        id: tableItemTextComponent
        Rectangle {
            property var holder: parent
            property bool selected: {
                if(multiSelectMode) {
                    return multiSelectCond(holder.itemRow,holder.itemData)
                } else {
                    return currentSelectRow === holder.itemRow
                }
            }
            color: selected ? "#EEEEEE" : (currentHoverRow === holder.itemRow ? "#F0F0F0" : "transparent")
            border {
                color: "#F0F0F0"
                width: 1
            }
            Text {
                id: tableItemDefaultText
                text: holder.itemData
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
                font: itemFont
            }
        }
    }

    Text {
        id: itemFontHolder
        font {
            pixelSize: rfs(14)
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
        interactive: false
        clip: true

        delegate: Rectangle {
            id: headerItemDelegate
            width: modelData.visible ? modelData.itemWidth : 0
            height: titleBarHeight
            color: index % 2 ?  "#FFFFFF": "#F0F0F0"

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
                        if(modelData.itemWidth <= modelData.minWidth && deltaX < 0) {
                            return;
                        }
                        modelData.itemWidth += deltaX;
                        headerModel.headerWidth += deltaX
                        tableView.forceLayout()
                    }
                    onDoubleClicked: {
                        headerModel.setItemWidthAt(index,tableModel.resizeToContents(index,itemFont))
                        tableView.forceLayout()
                    }
                }
            }

            Text {
                text: modelData.title
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: modelData.titleFontSize
                elide: Text.ElideRight
            }
        }
    }

    TableView {
        id: tableView
        contentX: headerListView.contentX
        anchors.fill: parent
        anchors.topMargin: titleBarHeight
        rowHeightProvider: function () {
            return rowHeight
        }

        columnWidthProvider: function columnWidth(column) {
            return headerModel.headerItems[column].itemWidth;
        }
        contentWidth: headerModel.headerWidth
        flickableDirection: TableView.VerticalFlick
        clip: true
        rowSpacing: -1
        columnSpacing: -1
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

        delegate: Loader {
            id: tableItemLoader
            property var itemData: modelData
            property var itemRow: row
            property var itemColumn: column
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
                onClicked: {
                    if(!selectEnable) {
                        return
                    }
                    if(multiSelectMode) {
                        switchSelect(row)
                    } else {
                        currentSelectRow = row
                    }
                    mouse.accepted = false
                }

                onDoubleClicked: {
                    control.rowDoubleClicked(row)
                    mouse.accepted = false
                }
            }
            sourceComponent: itemComponentSelector(row,column)
        }
    }
}

