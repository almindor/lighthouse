import QtQuick 2.0
import Sailfish.Silica 1.0

BackgroundItem {

    property int cpuUse
    property int memUse
    property string procName
    property bool largeScreen: Screen.sizeCategory === Screen.Large ||
                               Screen.sizeCategory === Screen.ExtraLarge

    anchors {
        left: parent.left
        right: parent.right
        margins: Theme.paddingLarge
    }
    height: greyBar.height
    Label {
        anchors {
            left: parent.left
        }

        id: itemLabel
        text: largeScreen ? procName : (isPortrait ? procName.substring(0,12) : procName)
    }

    Text {
        id: cpuLabel
        text: qsTr("cpu: ") + cpuUse + "%"
        color: Theme.highlightColor
        font.pointSize: largeScreen ? 28 : 12
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
        }
    }

    Text {
        id: memLabel
        text: qsTr("mem: ") + memUse + "%"
        color: Theme.secondaryHighlightColor
        font.pointSize: largeScreen ? 28 : 12
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
            topMargin: cpuLabel.height + 1
        }
    }

    Rectangle {
        id: greyBar
        anchors {
            right: parent.right
            top: parent.top
        }

        opacity: 0.5
        width: largeScreen ? 400 : 200
        height: cpuLabel.height + memLabel.height + 1
        color: "dimgrey"
    }

    Rectangle {
        id: cpuBar
        anchors {
            right: parent.right
            top: parent.top
        }

        width: cpuUse * 2
        height: cpuLabel.height
        color: Theme.highlightColor
    }

    Rectangle {
        id: memBar
        anchors {
            right: parent.right
            top: parent.top
            topMargin: cpuBar.height + 1
        }

        width: memUse * 2
        height: memLabel.height
        color: Theme.secondaryHighlightColor
    }
}
