import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.5
import "../../Src/Controls"
import "../../Src/Effects"

import Neo.Quick 1.0
import Neo.Tests 1.0
//import Neo.Net 1.0

NeoAppWindow {
    id: window


//    ShowNeoTableView {
//        anchors.fill: parent
//    }


    ShowMovableListItemView {
        anchors.fill: parent
    }
}
