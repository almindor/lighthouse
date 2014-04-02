import QtQuick 2.0
import Sailfish.Silica 1.0


Page {
    id: page

    SilicaListView {

        header: PageHeader {
            title: "Processes"
        }

        PullDownMenu {
            MenuItem {
                text: "Is sorted by: " + process.sortBy
                onClicked: process.nextSortBy()
            }
        }

        anchors.fill: parent
        spacing: 2
        model: process
        VerticalScrollDecorator {}

        delegate: Item {
            anchors {
                left: parent.left
                right: parent.right
                margins: Theme.paddingLarge
            }
            height: Theme.itemSizeExtraSmall
            Label {
                anchors {
                    left: parent.left
                }

                id: itemLabel
                text: name
            }

            Rectangle {
                anchors {
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                }

                width: 200
                height: 5
                color: "dimgrey"

                Text {
                    text: cpuUsage+"%"
                    color: Theme.highlightColor
                    font.pointSize: 12
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        bottom: parent.top
                    }
                }
            }

            Rectangle {
                anchors {
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                }

                width: cpuUsage * 2
                height: 5
                color: Theme.highlightColor
            }
        }
    }
}
