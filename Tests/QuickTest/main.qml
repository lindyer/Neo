import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.5
import "../../Src/Controls"
import "../../Src/Effects"

import Neo.Quick 1.0
import Neo.Tests 1.0
//import Neo.Net 1.0
import NeoQuick 1.0

NeoAppWindow {
    id: window

    ShowNeoTableView {
        anchors.fill: parent
    }

//    Timer {
//        repeat: true
//        interval: 2000
//        running: true
//        onTriggered: {
//            print(Cursor.pos())
//        }
//    }

//    TextEdit {
//        anchors.fill: parent
//        focus: true
//        Component.onCompleted: {
//            forceActiveFocus()
//        }
//    }

//    NeoFontHolder {
//        text: "I am font holder"
//        anchors.centerIn: parent
//        Component.onCompleted: {
//            print("#2>>")
//        }
//    }

//    ShowMovableListItemView {
//        anchors.fill: parent
//    }
//    TmpCopy {

//    }
}
