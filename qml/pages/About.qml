import QtQuick 2.0
import Sailfish.Silica 1.0


Page {
    id: page

    Column {
        width: page.width
        spacing: Theme.paddingLarge

        PageHeader {
            id: header
            title: "About"
        }

        Label {
            anchors {
                left: parent.left
                right: parent.right
                margins: Theme.paddingLarge
            }
            wrapMode: Text.WordWrap
            color: Theme.primaryColor
            text: "Copyright 2014 © by Aleš Katona. All rights reserved."
        }

        Label {
            anchors {
                left: parent.left
                right: parent.right
                margins: Theme.paddingLarge
            }
            wrapMode: Text.WordWrap
            color: Theme.primaryColor
            text: "Battery changes by Lavaux Gilles."
        }

        Text {
            anchors {
                left: parent.left
                right: parent.right
                margins: Theme.paddingLarge
            }
            color: Theme.secondaryColor
            font.pointSize: 16
            wrapMode: Text.WordWrap
            text: "Lighthouse is free software licensed under the GPL v3 available at <a href='https://github.com/almindor/lighthouse'>github</a>. Phone controls DBus code inspired by <a href='https://github.com/ejjoman/SailfishReboot'>SailfishReboot</a>."
            linkColor: Theme.secondaryHighlightColor
            onLinkActivated: {
                 Qt.openUrlExternally(link);
            }
        }

    }
}
