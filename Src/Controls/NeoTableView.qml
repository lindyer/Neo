import QtQuick 2.12
import QtQuick.Controls 2.4

import Neo.Quick 1.0


Item {
    id: control

    default property alias tableViewChildren: tableView.children

    //export alias property
    property alias headerModel: headerListView.model // it need two field: Title and Width
    property alias tableModel: tableView.model
    property alias tableView: tableView

    //header properties
    property color headerSplitColor: "#D8D8D8"
    property int dragedIndex: -1
    property int dragedMoveX: 0


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
    //    property int contentWidth: 0
    property var selectedRows: []
    property int titleBarHeight: rh(30)
    property var multiSelectCond: function(row,cellValue) { return selectedRows.indexOf(row) >= 0 }
    /**
     * tableItemComponentSelector:
     * property var holder: parent
     * parent point to delegate loader which hold itemData,itemRow,itemColumn
     */
    property var tableItemComponentSelector: function(row,column) {
        return tableItemDefaultComponent
    }

    /*
     * headerItemComponentSelector
     * property var holder: parent
     * parent point to delegate loader which hold itemData,itemColumn
     */
    property var headerItemComponentSelector: function(column,itemData) {
        return headerItemDefaultComponent
    }

    property alias tableItemDefaultComponent: tableItemDefaultComponent
    property alias headerItemDefaultComponent: headerItemDefaultComponent

    //scrollbar
    property alias horizontalScrollBar: horizontalScrollBar
    property alias verticalScrollBar: verticalScrollBar
    property int horizontalScrollBarPolicy: ScrollBar.AsNeeded
    property int verticalScrollBarPolicy: ScrollBar.AsNeeded

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
        id: headerItemDefaultComponent
        Rectangle {
            id: headerItemDelegate
            property var holder: parent
            border { width: 1; color: "#D0D0D0" }
            color: "#F0F0F0"
            Text {
                text: holder.itemData.title
                width: parent.width
                leftPadding: 8
                rightPadding: 8
                horizontalAlignment: holder.itemData.titleHorizontalAlignment
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: holder.itemData.titleFontSize
                elide: Text.ElideRight
            }
        }
    }

    Component {
        id: tableItemDefaultComponent
        Rectangle {
            property var holder: parent
            color: holder.selected ? "#EEEEEE" : (currentHoverRow === holder.itemRow ? "#F0F0F0" : "transparent")
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
    ListView {
        id: headerListView
        width: parent.width
        height: titleBarHeight
        orientation: ListView.Horizontal
        contentX: tableView.contentX
        interactive: false
        clip: true
//        moveDisplaced: Transition {
//            NumberAnimation { properties: "x,y"; duration: 300 }
//        }
        delegate: Loader {
            id: headerItemLoader
            //define properties which can be accessed by loader.item
            property var itemData: modelData
            property var itemColumn: index
            clip: true
            onItemDataChanged: {
                print(index,itemData.title)
            }

            width: modelData.visible ? modelData.itemWidth : 1
            height: titleBarHeight

            sourceComponent: headerItemComponentSelector(index,modelData)

            MouseArea {
                id: wholeMouseArea
                anchors.fill: parent
                cursorShape: pressed ? Qt.PointingHandCursor : Qt.ArrowCursor

            }

            Item {
                width: rw(5)
                height: parent.height
                anchors {
                    right: parent.right
                }
                enabled: !wholeMouseArea.pressed
                //rise and make headerItemMouseArea handle first
                z: headerItemLoader.item.z + 1
                MovableArea {
                    id: headerItemMouseArea
                    enabled: modelData.visible && !wholeMouseArea.pressed
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
            return headerModel.headerItems[column].visible ? headerModel.headerItems[column].itemWidth : 1;
        }
        contentWidth: headerModel.headerWidth
        flickableDirection: TableView.VerticalFlick
        clip: true
        rowSpacing: -1
        columnSpacing: -1

        ScrollBar.horizontal: NeoScrollBar {
            id: horizontalScrollBar
            orientation: Qt.Horizontal
            parent: tableView.parent
            policy: horizontalScrollBar.width < tableView.contentWidth ? ScrollBar.AlwaysOn : horizontalScrollBarPolicy
            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom
                bottomMargin: rw(-12)
            }
        }

        ScrollBar.vertical: NeoScrollBar {
            id: verticalScrollBar
            policy: verticalScrollBar.height < tableView.contentHeight ? ScrollBar.AlwaysOn : verticalScrollBarPolicy
            parent: tableView.parent
            anchors {
                top: tableView.top
                bottom: tableView.bottom
                right: tableView.right
                rightMargin: rw(-12)
            }
        }

        delegate: Loader {
            id: tableItemLoader
            property var itemData: modelData
            property var itemRow: row
            property var itemColumn: column

            property bool selected: {
                if(multiSelectMode) {
                    return multiSelectCond(row,modelData)
                } else {
                    return currentSelectRow === row
                }
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
            sourceComponent: tableItemComponentSelector(row,column)
        }
    }
}

