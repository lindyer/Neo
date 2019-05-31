import QtQuick 2.12

AbstractTheme {
    title: "Dark"

    color: "#333333"

    rippleColor: "#33000000"

    function updatePalette(palette) {
        palette.alternateBase = ""
        palette.base = "#333333"
        palette.brightText = ""
        palette.button = "red"
        palette.buttonText = "white"
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
        palette.toolTipBase = "#333333"
        palette.toolTipText = "white"
        palette.window = "#333333"
        palette.windowText = "white"
    }
}
