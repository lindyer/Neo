import QtQuick 2.12

AbstractTheme {
    title: "Light"

    color: "white"

    rippleColor: "#10000000"

    function updatePalette(palette) {
        palette.alternateBase = ""
        palette.base = "white"
        palette.brightText = ""
        palette.button = "#CCCCCC"
        palette.buttonText = "red"
        palette.dark = ""
        palette.highlight = ""
        palette.highlightedText = ""
        palette.light = ""
        palette.link = ""
        palette.linkVisited = ""
        palette.mid = ""
        palette.midlight = ""
        palette.shadow = ""
        //palette.text = ""
        palette.toolTipBase = "white"
        palette.toolTipText = "#333333"
        palette.window = "white"
        palette.windowText = "#333333"
    }
}
