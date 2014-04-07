import QtQuick 2.0
import Sailfish.Silica 1.0


Page {
    id: page

    Column {
        width: page.width
        spacing: Theme.paddingLarge
        PageHeader {
            title: "Memory Usage"
        }

        ProgressCircleBase {
            width: parent.width / 2
            height: width
            anchors.horizontalCenter: parent.horizontalCenter
            value: 1 - (memory.free / memory.total)
            borderWidth: 2
            progressColor: Theme.highlightColor

            Column {
                width: parent.width * 0.7
                anchors.centerIn: parent

                Text {
                    width: parent.width
                    color: Theme.highlightColor
                    font.pixelSize: Theme.fontSizeExtraLarge
                    horizontalAlignment: Text.AlignHCenter
                    text: (memory.total - memory.free) + " kB"
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
                    text: memory.total + " kB"
                }
            }


        }

        ProgressBar {
            minimumValue: 0
            maximumValue: 100
            value: (100 - memory.summaryValue)
            label: memory.free + " kB free out of " + memory.total + " kB total"
            anchors {
                left: parent.left
                right: parent.right
                margins: Theme.paddingLarge
            }
        }
    }
}
