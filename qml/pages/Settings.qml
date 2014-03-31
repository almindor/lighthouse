import QtQuick 2.0
import Sailfish.Silica 1.0


Page {
    id: page

    Column {
        width: page.width
        spacing: Theme.paddingLarge
        PageHeader {
            title: "Settings"
        }

        Slider {
            minimumValue: 1
            maximumValue: 10
            stepSize: 1
            value: monitor.interval
            valueText: value + 's'
            label: "Update interval"
            onValueChanged: monitor.interval = value
            anchors {
                left: parent.left
                right: parent.right
                margins: Theme.paddingLarge
            }
        }

    }

}
