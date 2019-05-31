import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.5
import "../../Src/Controls"
import "../../Src/Effects"

import Neo.Quick 1.0
import Neo.Tests 1.0

NeoAppWindow {
    id: window

    TmpTableView {
        headerModel: NeoTableHeaderModel {
            headerItems: [
                NeoTableHeaderItem {
                    title: "列1"
                    itemWidth: 200
                    titleFontSize: 14
                },
                NeoTableHeaderItem {
                    title: "列2"
                    itemWidth: 80
                    titleFontSize: 10
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

//    Flow {
//        anchors.fill: parent
//        spacing: 8
//        Repeater {
//            model: 20
//            delegate: NeoButton {
//                hoverEnabled: true
//                NeoToolTip {
//                    visible: parent.hovered
//                    anchorDirection: NeoToolTip.AnchorBottom
//                }

//                onClicked: {
//                    if(index % 2) {
//                        turnToBlackTheme()
//                    } else {
//                        turnToLightTheme()
//                    }
//                }
//            }
//        }
//    }

}
