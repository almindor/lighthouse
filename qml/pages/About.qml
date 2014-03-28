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

        TextArea {
            readOnly: true
            wrapMode: TextEdit.WordWrap
            anchors {
                centerIn: parent.Center
                margins: Theme.paddingLarge
            }
            width: parent.width / 1.3
            height: parent.height / 1.5
            text: "Copyright 2014 © by Aleš Katona. All rights reserved."
        }

    }

}
