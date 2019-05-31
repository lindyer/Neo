import QtQuick 2.12
import QtQuick.Controls 2.12

import "../Effects"

Button {
    id: control

    text: "NeoButton"

    background: NeoDropShadow {
        dropShadow.samples: 16
        dropShadow.color: "#66000000"
        contentItem: Rectangle {
            color: palette.button
            NeoRipple {}
        }
    }
}
