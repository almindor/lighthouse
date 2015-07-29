import QtQuick 2.0
import Sailfish.Silica 1.0

Column {
    width: parent.width * 0.7
    anchors.centerIn: parent

    property var topVal
    property var botVal

    property string topUnit : ""
    property string botUnit : ""

    Text {
        width: parent.width
        color: Theme.highlightColor
        font.pixelSize: Theme.fontSizeExtraLarge
        horizontalAlignment: Text.AlignHCenter
        text: topVal + topUnit
    }

    Rectangle {
        color: Theme.highlightColor
        width: parent.width
        height: 2
        opacity: 0.4
    }

    Text {
        width: parent.width
        color: Theme.highlightColor
        opacity: 0.6
        font.pixelSize: Theme.fontSizeExtraLarge
        horizontalAlignment: Text.AlignHCenter
        text: botVal + botUnit
    }
}
