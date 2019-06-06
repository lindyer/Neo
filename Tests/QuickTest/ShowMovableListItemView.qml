import QtQuick 2.0
import "../../Src/Controls"

Item {
    NeoMovableListItemView {
        anchors.fill: parent
        model: ListModel {
            ListElement {
                content: "First"
                modelColor: "red"
            }
            ListElement {
                content: "Second"
                modelColor: "green"
            }
            ListElement {
                content: "Third"
                modelColor: "blue"
            }
        }
    }
}
