import QtQuick 2.12

Item {
    id: control

    property color color: theme.rippleColor

    anchors.fill: parent

    clip: true

    Rectangle {
        id: ripple
        color: control.color
        radius: width / 2
        visible: scaleAnimation.running

        ScaleAnimator {
            id: scaleAnimation
            target: ripple
            duration: 300
            from: 0
            to: 1
        }
    }


    MouseArea {
        anchors.fill: parent
        onPressed: {
            //计算波纹圆的最大半径
            var dx = control.width - mouseX
            var dy = control.height - mouseY
            //点击处到四角的距离
            var toLeftTop = Math.sqrt(Math.pow(mouseX,2) + Math.pow(mouseY,2))
            var toLeftBottom = Math.sqrt(Math.pow(mouseX,2)+ Math.pow(dy,2))
            var toRightTop = Math.sqrt(Math.pow(mouseY,2)+ Math.pow(dx,2))
            var toRightBottom = Math.sqrt(Math.pow(dy,2) + Math.pow(dx,2))
            var arr = [toLeftTop,toLeftBottom,toRightTop,toRightBottom]
            arr = arr.sort(function(a,b){ return b - a })
            ripple.width = ripple.height = arr[0] * 2
            ripple.x = mouseX - ripple.width / 2
            ripple.y = mouseY - ripple.width / 2
            scaleAnimation.start()
            mouse.accepted = false
        }
    }
}
