import QtQuick 2.12
import QtQuick.Layouts 1.12

/*
  Row {
        NeoWaitingHoodle {
            id: hoodle
        }

        NeoButton {
            text: "start"
            onClicked: hoodle.started()
        }

        NeoButton {
            text: "stop"
            onClicked: hoodle.stoped()
        }
   }
*/

Rectangle {
    id: control

    property int count: 5

    property int itemWidth: rw(12)

    property int itemHeight: itemWidth

    property int itemRadius: itemWidth / 2

    property color itemColor: "#FF4444"

    property int speed: 200

    signal started

    signal stoped

    function __selectDirection(seqUpFirstAnimation,seqDownFirstAnimation,index) {
        if(!(index % 2)) {
            seqDownFirstAnimation.start()
        } else {
            seqUpFirstAnimation.start()
        }
    }

    onStarted: visible = true

    onStoped: visible = false

    width: rw(130)

    height: rh(30)


    RowLayout {
        anchors.fill: parent
        Repeater {
            model: control.count
            delegate: Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Rectangle {
                    id: rect
                    width: itemWidth
                    height: itemHeight
                    anchors.horizontalCenter: parent.horizontalCenter
                    radius: itemRadius
                    color: itemColor

                    Connections {
                        target: control
                        onStarted: {
                            __selectDirection(seqUpFirstAnimation,seqDownFirstAnimation,index)
                        }
                        onStoped: {
                            seqUpFirstAnimation.stop()
                            seqDownFirstAnimation.stop()
                        }
                    }

                    SequentialAnimation {
                        id: seqDownFirstAnimation
                        loops: Animation.Infinite
                        YAnimator {
                            target: rect
                            from: 0
                            to: control.height - rect.height
                            duration: speed
                        }
                        YAnimator {
                            target: rect
                            from: control.height - rect.height
                            to: 0
                            duration: speed
                        }
                    }

                    SequentialAnimation {
                        id: seqUpFirstAnimation
                        loops: Animation.Infinite
                        YAnimator {
                            target: rect
                            from: control.height - rect.height
                            to: 0
                            duration: speed
                        }
                        YAnimator {
                            target: rect
                            from: 0
                            to: control.height - rect.height
                            duration: speed
                        }
                    }
                }
            }
        }
    }
}
