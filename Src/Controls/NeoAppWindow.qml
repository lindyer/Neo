import QtQuick 2.12
import QtQuick.Controls 2.12
import "../Themes"

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Neo Quick App")

    property AbstractTheme theme: themeList[0]

    property list<AbstractTheme> themeList: [
        LightTheme { id: lightTheme },
        DarkTheme { id: darkTheme }
    ]

    onThemeChanged: {
        theme.updatePalette(palette)
    }

    function turnToLightTheme() {
        if(theme.title === lightTheme.title) {
            return
        }
        theme = themeList[0]
    }

    function turnToBlackTheme() {
        if(theme.title === darkTheme.title) {
            return
        }
        theme = themeList[1]
    }

    function rw(value) {
        return value
    }

    function rh(value) {
        return value
    }

    function rfs(value) {
        return value
    }
}
