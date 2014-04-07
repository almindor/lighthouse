import QtQuick 2.0
import Sailfish.Silica 1.0


Page {
    id: page

    PageHeader {
        id: header
        title: "About"
    }

    Label {
        anchors {
            centerIn: parent.Center
            horizontalCenter: parent.horizontalCenter
        }
        color: Theme.primaryColor
        text: "Copyright 2014 © by Aleš Katona. All rights reserved. Phone controls DBus code inspired by <a href='https://github.com/ejjoman/SailfishReboot'>SailfishReboot</a>"
        onLinkActivated: {
             Qt.openUrlExternally(link);
        }
    }

    Text {
        anchors {
            centerIn: parent.Center
            horizontalCenter: parent.horizontalCenter
        }
        color: Theme.primaryColor
        font.pointSize: theme.fontSizeMedium
        text: "Battery changes: Lavaux Gilles"
        Text {
            anchors {
                left: parent.left
                top: parent.bottom
            }
            color: Theme.primaryColor
            font.pointSize: 16
            text: "gl custom version: 0.5.6"
        }
    }

}
