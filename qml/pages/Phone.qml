import QtQuick 2.0
import Sailfish.Silica 1.0


Page {
    id: page

    PageHeader {
        id: header
        title: "Phone Controls"
    }

    Column {
        width: page.width
        spacing: Theme.paddingLarge
        anchors.verticalCenter: parent.verticalCenter

        Button {
            text: "Reboot"
            anchors {
                horizontalCenter: parent.horizontalCenter
                margins: Theme.paddingLarge
            }
            onClicked: monitor.reboot();
        }

        Button {
            text: "Shutdown"
            anchors {
                horizontalCenter: parent.horizontalCenter
                margins: Theme.paddingLarge
            }
            onClicked: monitor.shutdown();
        }

    }

}
