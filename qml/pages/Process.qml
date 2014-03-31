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
                text: "Sort by: " + process.sortBy
                onClicked: process.nextSortBy()
            }
        }

        anchors.fill: parent
        spacing: 2
        model: process
        VerticalScrollDecorator {}

        delegate: Item {
            height: Theme.itemSizeExtraSmall
            Label {
                id: itemLabel
                text: descr
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }
            }
        }
    }
}
