import QtQuick 2.0
import Sailfish.Silica 1.0

BackgroundItem {
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
        text: name
    }

    Text {
        id: cpuLabel
        text: qsTr("cpu: ") + cpuUsage + "%"
        color: Theme.highlightColor
        font.pointSize: 12
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
        }
    }

    Text {
        id: memLabel
        text: qsTr("mem: ") + memoryUsage + "%"
        color: Theme.secondaryHighlightColor
        font.pointSize: 12
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
        width: 200
        height: cpuLabel.height + memLabel.height + 1
        color: "dimgrey"
    }

    Rectangle {
        id: cpuBar
        anchors {
            right: parent.right
            top: parent.top
        }

        width: cpuUsage * 2
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

        width: memoryUsage * 2
        height: memLabel.height
        color: Theme.secondaryHighlightColor
    }
}
