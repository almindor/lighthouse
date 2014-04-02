import QtQuick 2.0
import Sailfish.Silica 1.0


Page {
    id: page

    Column {
        width: page.width
        spacing: Theme.paddingLarge
        PageHeader {
            title: "About"
        }


        Label {
            anchors {
                centerIn: parent.Center
                horizontalCenter: parent.horizontalCenter
            }
            color: Theme.primaryColor
            text: "Copyright 2014 © by Aleš Katona."
        }
        Text {
            anchors {
                centerIn: parent.Center
                horizontalCenter: parent.horizontalCenter
            }
            color: Theme.primaryColor
            font.pointSize: theme.fontSizeMedium
            text: "Small changes: Lavaux Gilles"
            Text {
                anchors {
                    left: parent.left
                    top: parent.bottom
                }
                color: Theme.primaryColor
                font.pointSize: 16
                text: "gl custom version: 0.5.0"
            }
        }
    }

}
