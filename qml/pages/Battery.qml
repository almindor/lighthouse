import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page


    Column {
        width: page.width
        spacing: Theme.paddingLarge
        PageHeader {
            title: "Battery Usage"
        }

        ProgressCircleBase {
            width: parent.width / 2
            height: width
            anchors.horizontalCenter: parent.horizontalCenter
            value: battery.level/100.1
            borderWidth: 2
            progressColor: Theme.highlightColor

            Column {
                width: parent.width * 0.7
                anchors.centerIn: parent

                Text {
                    width: parent.width
                    color: Theme.highlightColor
                    opacity: 0.6
                    font.pixelSize: Theme.fontSizeMedium
                    horizontalAlignment: Text.AlignHCenter
                    text: battery.status
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
                    font.pixelSize: Theme.fontSizeMedium
                    horizontalAlignment: Text.AlignHCenter
                    text: battery.level
                }
            }

        }

        Text {
            width: parent.width
            color: Theme.highlightColor
            font.pixelSize: Theme.fontSizeSmall
            horizontalAlignment: Text.AlignHCenter
            text: "Health: "+battery.health
        }

        Text {
            width: parent.width
            color: Theme.highlightColor
            font.pixelSize: Theme.fontSizeSmall
            horizontalAlignment: Text.AlignHCenter
            text: "Techno: "+battery.techno
        }
    }
}

