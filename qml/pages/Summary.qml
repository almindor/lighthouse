import QtQuick 2.0
import Sailfish.Silica 1.0


Page {
    id: page

    SilicaFlickable {
        anchors.fill: parent

        PullDownMenu {
            MenuItem {
                text: "About"
                onClicked: pageStack.push(Qt.resolvedUrl("About.qml"))
            }
            MenuItem {
                text: "Settings"
                onClicked: pageStack.push(Qt.resolvedUrl("Settings.qml"))
            }
        }

        contentHeight: column.height

        Column {
            id: column

            width: page.width
            spacing: Theme.paddingLarge
            PageHeader {
                title: "Summary"
            }

            Row {
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    margins: Theme.paddingLarge
                }

                Label {
                    color: Theme.primaryColor
                    text: "Uptime: "
                }

                Label {
                    color: Theme.highlightColor
                    text: monitor.uptime
                }
            }

            Button {
                text: "CPUs: " + cpu.summaryValue + "%"
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }
                onClicked: pageStack.push(Qt.resolvedUrl("CPU.qml"))
            }


            Button {
                text: "Memory: " + memory.summaryValue + "%"
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }
                onClicked: pageStack.push(Qt.resolvedUrl("Memory.qml"))
            }

            Button {
                text: "Processes: " + process.summaryValue
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }
                onClicked: pageStack.push(Qt.resolvedUrl("Process.qml"))
            }

        }
    }
}


