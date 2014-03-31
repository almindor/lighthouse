import QtQuick 2.0
import Sailfish.Silica 1.0


Page {
    id: page

    Column {
        width: page.width
        spacing: Theme.paddingLarge
        PageHeader {
            title: "CPU Usage"
        }

        ProgressCircleBase {
            width: parent.width / 2
            height: width
            anchors.horizontalCenter: parent.horizontalCenter
            value: cpu.summaryValue / 100
            borderWidth: 2
            progressColor: Theme.highlightColor

            Text {
                width: parent.width
                anchors.centerIn: parent
                color: Theme.highlightColor
                font.pixelSize: Theme.fontSizeHuge
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: cpu.summaryValue + '%'
            }

        }

        Repeater {
            model: (cpu.usage.length - 1)

            ProgressBar {
                minimumValue: 0
                maximumValue: 100
                value: cpu.usage[index + 1]
                valueText: value + '%'
                visible: (cpu.usage.length > 0)
                label: "CPU" + (index + 1);
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }
            }
        }

    }
}
