import QtQuick 2.12
import QtQuick.Controls 2.5
import "../../Src/Controls"
import "../../Src/Effects"

import Neo.Quick 1.0
import Neo.Tests 1.0

Item {
    anchors.fill: parent
    Component {
        id: tableItemButtonComponent
        Rectangle {
            property var holder: parent
            property bool selected: {
                if(tableView.multiSelectMode) {
                    return tableView.multiSelectCond(holder.itemRow,holder.itemData)
                } else {
                    return tableView.currentSelectRow === holder.itemRow
                }
            }
            color: selected ? "#EEEEEE" : (tableView.currentHoverRow === holder.itemRow ? "#F0F0F0" : "transparent")
            border {
                color: "#F0F0F0"
                width: 1
            }
            NeoButton {
                id: tableItemDefaultText
                text: holder.itemData
                anchors.fill: parent
                anchors.margins: 4
                font: tableView.itemFont
                onClicked: {
                    print(holder.itemRow,holder.itemColumn)
                }
            }
        }
    }

    Component {
        id: headerItemButtonComponent
        Rectangle {
            property var holder: parent
            color: "red"
            border {
                color: "#F0F0F0"
                width: 1
            }



            NeoButton {
                id: tableItemDefaultText
                text: holder.itemData.title + holder.itemData.extraData.otherData
                anchors.fill: parent
                anchors.margins: 4
                font: tableView.itemFont
                onClicked: {
                    print(holder.itemColumn,holder.itemData.extraData)
                }
            }
        }
    }

    Component {
        id: headerItemActionComponent
        Action {
            property int index
            checkable: true
            onToggled: {
                neoTableHeaderModel.setItemVisibleAt(index,checked)
                tableView.tableView.forceLayout()
            }
        }
    }

    NeoMenu {
        id: headerMenu
        title: "显示的列"
    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onClicked: {
            if(mouse.button === Qt.RightButton) {
                headerMenu.popup()
            }
        }
    }

    NeoTableView {
        id: tableView
        tableItemComponentSelector: function(row,column) {
            if(row === 1 && column === 1) {
                return tableItemButtonComponent
            } else {
                return tableItemDefaultComponent
            }
        }
        headerItemComponentSelector: function(column) {
            if(column === 1) {
                return headerItemButtonComponent
            } else {
                return headerItemDefaultComponent
            }
        }

        headerModel: NeoTableHeaderModel {
            id: neoTableHeaderModel
            Component.onCompleted: {
                for(var index in headerItems) {
                    var item = headerItems[index]
                    var action = headerItemActionComponent.createObject(headerMenu,{index:index,text:item.title,checked:item.visible})
                    headerMenu.addAction(action)
                }
            }

            headerItems: [
                NeoTableHeaderItem {
                    title: "-列1"
                    itemWidth: 200
                    titleFontSize: 14
                    titleHorizontalAlignment: Text.AlignRight
                },
                NeoTableHeaderItem {
                    title: "列2"
                    itemWidth: 180
                    titleFontSize: 10
                    //you can expand what you want by extraData
                    extraData: { "otherData": " Look At Me ^-^" }
                },
                NeoTableHeaderItem {
                    title: "列3"
                    itemWidth: 100
                    titleFontSize: 12
                },
                NeoTableHeaderItem {
                    title: "列4"
                    itemWidth: 60
                    titleFontSize: 16
                }
            ]
        }

        tableModel: TableModelTest {
            id: tableModel
        }
        anchors.fill: parent
        anchors.margins: 20
    }
}
