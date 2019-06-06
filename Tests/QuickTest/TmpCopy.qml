import QtQuick 2.5
import QtQuick.Window 2.0
import QtQml.Models 2.2
import QtQuick.Controls 2.2

Window {
    visible: true
    width: 300
    height: 400
    title: qsTr("List View Test")

    property bool dropAreaOneEntered: false
    property bool dropAreaTwoEntered: false
    property int listNumer: 0

    Rectangle {
        id: root
        width: parent.width
        height: parent.height

        ListView {
            id: displayListOne
            width: 100; height: parent.height
            model: displayListOneModel

            property int dragItemIndex: -1

            moveDisplaced: Transition {
                NumberAnimation{
                    properties: "x,y"
                    duration: 200
                }
            }
        }

        ListView {
            id: displayListTwo
            width: 100; height: parent.height
            anchors.right: parent.right
            model: displayListTwoModel

            property int dragItemIndex: -1

            moveDisplaced: Transition {
                NumberAnimation{
                    properties: "x,y"
                    duration: 200
                }
            }
        }

        // ..........................................................

        ListModel {
            id: itemsListModel
            // @disable-check M16
            ListElement { colore: "blue"; key: true }
            // @disable-check M16
            ListElement { colore: "orange"; key: false }
            // @disable-check M16
            ListElement { colore: "blue"; key: true }
            // @disable-check M16
            ListElement { colore: "orange"; key: false }
            // @disable-check M16
            ListElement { colore: "blue"; key: true }
            // @disable-check M16
            ListElement { colore: "orange"; key: false }
            // @disable-check M16
            ListElement { colore: "blue"; key: true }
            // @disable-check M16
            ListElement { colore: "orange"; key: false }
        }

        // ..........................................................

        DelegateModel {
            id: displayListOneModel

            model: itemsListModel
            delegate: MouseArea {
                id: mouseOneArea
                width: 100; height: 100

                property bool held: false

                drag.target: held ? itemOneBlock : undefined
                drag.axis: Drag.XAndYAxis

                property var draggedItem: null

                onPressAndHold: {
                    held = true
                    listNumer = 1
                    displayListOne.dragItemIndex = index
                }
                onReleased: {
                    listNumer = 0
                    held = false
                }

                Rectangle {
                    id: itemOneBlock
                    anchors { horizontalCenter: parent.horizontalCenter; verticalCenter: parent.verticalCenter }
                    width: 100
                    height: 100

                    color: colore
                    opacity: mouseOneArea.held ? 0.8 : 1.0

                    Drag.active: mouseOneArea.held
                    Drag.source: mouseOneArea
                    Drag.hotSpot.x: width / 2
                    Drag.hotSpot.y: height / 2

                    states: State{
                        when: mouseOneArea.held
                        ParentChange { target: itemOneBlock; parent: root }
                        AnchorChanges {
                            target: itemOneBlock
                            anchors { horizontalCenter: undefined; verticalCenter: undefined }
                        }
                    }
                }

                DropArea {
                    id: listOneDropArea
                    anchors.fill: parent

                    property bool flag: false

                    onExited: dropAreaOneEntered = false
                    onEntered: {
                        dropAreaOneEntered = true
                        if (dropAreaOneEntered & ! dropAreaTwoEntered){
                            if (listNumer == 1){
                                displayListOneModel.items.move(drag.source.DelegateModel.itemsIndex,
                                                               mouseOneArea.DelegateModel.itemsIndex)
                            }
                        }
                    }
                    onDropped: {
                        console.log("onDropped has been executed!")
                    }
                }
            }

            groups: [
                DelegateModelGroup {
                    includeByDefault: true
                    name: "listOne"
                }
            ]

            filterOnGroup: "listOne"

            Component.onCompleted: {
//                var rowCount = itemsListModel.count;
//                for( var i = 0; i < items.count; i++ ) {
//                    var entry = items.get(i).model;
//                    if((entry.key != true)) {
//                        items.removeGroups(i,1,"listOne")
//                    }
//                }
            }
        }

        DelegateModel {
            id: displayListTwoModel

            model: itemsListModel
            delegate: MouseArea {
                id: mouseTwoArea
                width: 100; height: 100

                property bool held: false

                drag.target: held ? itemTwoBlock : undefined
                drag.axis: Drag.XAndYAxis

                onPressAndHold: {
                    held = true
                    listNumer = 2
                    displayListTwo.dragItemIndex = index
                    console.log("current index is "+index)
                }
                onReleased: {
                    listNumer = 0
                    held = false
                }

                Rectangle {
                    id: itemTwoBlock
                    anchors { horizontalCenter: parent.horizontalCenter; verticalCenter: parent.verticalCenter }
                    width: 100
                    height: 100

                    color: colore
                    opacity: mouseTwoArea.held ? 0.8 : 1.0

                    Drag.active: mouseTwoArea.held
                    Drag.source: mouseTwoArea
                    Drag.hotSpot.x: width / 2
                    Drag.hotSpot.y: height / 2

                    states: State{
                        when: mouseTwoArea.held
                        ParentChange { target: itemTwoBlock; parent: root }
                        AnchorChanges {
                            target: itemTwoBlock
                            anchors { horizontalCenter: undefined; verticalCenter: undefined }
                        }
                    }
                }

                DropArea {
                    id: listTwoDropArea
                    anchors.fill: parent
                    onExited: dropAreaTwoEntered = false
                    onEntered: {
                        dropAreaTwoEntered = true
                        if (dropAreaTwoEntered & ! dropAreaOneEntered){
                            if (listNumer == 2){
                                displayListTwoModel.items.move(drag.source.DelegateModel.itemsIndex,
                                                               mouseTwoArea.DelegateModel.itemsIndex)
                            }
                        }
                    }
                    onDropped: {
                        console.log("Flip the key value here!")
                    }
                }
            }

            groups: [
                DelegateModelGroup {
                    includeByDefault: true
                    name: "listTwo"
                }
            ]
            filterOnGroup: "listTwo"
            Component.onCompleted: {
                var rowCount = itemsListModel.count;
                for( var i = 0; i < items.count; i++ ) {
                    var entry = items.get(i).model;
                    if((entry.key != false)) {
                        items.removeGroups(i,1,"listTwo")
                    }
                }
            }
        }
    }
}
