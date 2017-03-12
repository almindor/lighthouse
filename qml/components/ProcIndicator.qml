import QtQuick 2.0
import Sailfish.Silica 1.0

BackgroundItem {

    property bool withName: true
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
        visible: withName
        anchors {
            left: parent.left
        }

        id: itemLabel
        text: largeScreen ? procName : (isPortrait ? procName.substring(0,12) : procName)
    }

    Text {
        id: cpuLabel
        z: 99
        text: qsTr("cpu: ") + cpuUse + "%"
        color: Theme.primaryColor
        font.pointSize: largeScreen ? Theme.fontSizeSmall : Theme.fontSizeExtraSmall
        anchors {
            horizontalCenter: greyBar.horizontalCenter
            top: parent.top
        }
    }

    Text {
        id: memLabel
        text: qsTr("mem: ") + memUse + "%"
        z: 99
        color: Theme.secondaryColor
        font.pointSize: largeScreen ? Theme.fontSizeSmall : Theme.fontSizeExtraSmall
        anchors {
            horizontalCenter: greyBar.horizontalCenter
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
        width: withName ? parent.width / 2.0 : parent.width
        height: cpuLabel.height + memLabel.height + 1
        color: "dimgrey"
    }

    Rectangle {
        id: cpuBar
        anchors {
            right: parent.right
            top: parent.top
        }

        width: cpuUse / 100.0 * greyBar.width
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

        width: memUse / 100.0 * greyBar.width
        height: memLabel.height
        color: Theme.secondaryHighlightColor
    }
}
