import QtQuick 2.12
import QtQuick.Controls 2.12
import "../Effects"

ToolTip {
    id: control

    property int anchorDirection: NeoToolTip.AnchorDirection.AnchorTop

    property color backColor: palette.toolTipBase

    property color textColor: palette.toolTipText

    property int borderWidth: 0

    property bool indicatorVisible: true

    property real indicatorOffset: rw(-3)

    property real indicatorRawWidth: rw(10)

    property real offsetParent: rw(8)

    property real indicatorRadius: rw(2)

    property real radius: rw(4)

    enum AnchorDirection {
        AnchorTop,
        AnchorBottom,
        AnchorLeft,
        AnchorRight
    }

    onVisibleChanged: {
        if(anchorDirection === NeoToolTip.AnchorTop) {
            x = (parent.width - width) / 2
            y = -offsetParent - height
        } else if(anchorDirection === NeoToolTip.AnchorBottom) {
            x = (parent.width - width) / 2
            y = offsetParent + height
        } else if(anchorDirection === NeoToolTip.AnchorLeft) {
            x = -offsetParent - width
            y = (parent.height - height) / 2
        } else if(anchorDirection === NeoToolTip.AnchorRight) {
            x = width
            y = (parent.height - height) / 2
        }
    }

    text: "NeoToolTip"

    font { bold: true; pixelSize: rfs(14) }

    background: NeoDropShadow {
        contentItem: Rectangle {
            id: backRect
            color: backColor
            radius: control.radius
            Rectangle {
                id: indicator
                color: backColor
                width: indicatorRawWidth
                height: width
                rotation: 45
                radius: indicatorRadius
                visible: indicatorVisible
                smooth: true
                antialiasing: true

                Connections{
                    target: control

                    onYChanged: {
                        //BUG!!! FIXME,why not effect
                        if(anchorDirection === NeoToolTip.AnchorTop || anchorDirection === NeoToolTip.AnchorBottom) {
                            indicator.anchors.verticalCenter = undefined
                            indicator.anchors.horizontalCenter = backRect.horizontalCenter
                            if(control.y > 0) {
                                indicator.anchors.bottom = undefined
                                indicator.anchors.top = backRect.top
                                indicator.anchors.topMargin = indicatorOffset
                            } else if(control.y < 0) {
                                indicator.anchors.top = undefined
                                indicator.anchors.bottom = backRect.bottom
                                indicator.anchors.bottomMargin = indicatorOffset
                            }
                        }
                    }
                    onXChanged: {
                        if(anchorDirection === NeoToolTip.AnchorLeft) {
                            indicator.anchors.horizontalCenter = undefined
                            indicator.anchors.verticalCenter = backRect.verticalCenter
                            if(control.x > 0) {
                                indicator.anchors.right = undefined
                                indicator.anchors.left = backRect.left
                                indicator.anchors.leftMargin = indicatorOffset
                            } else {
                                indicator.anchors.left = undefined
                                indicator.anchors.right = backRect.right
                                indicator.anchors.rightMargin = indicatorOffset
                            }
                        } else if(anchorDirection === NeoToolTip.AnchorRight) {
                            indicator.anchors.verticalCenter = backRect.verticalCenter
                            if(control.x > 0) {
                                indicator.anchors.right = undefined
                                indicator.anchors.left = backRect.left
                                indicator.anchors.leftMargin = indicatorOffset
                            } else {
                                indicator.anchors.left = undefined
                                indicator.anchors.right = backRect.right
                                indicator.anchors.rightMargin = indicatorOffset
                            }
                        }
                    }
                }
            }
        }
    }

    contentItem: Text {
        text: control.text
        font: control.font
        color: textColor
    }
}
