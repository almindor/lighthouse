import QtQuick 2.0
import Sailfish.Silica 1.0


Page {
    id: page

    PageHeader {
        id: header
        title: "About"
    }

    Label {
         textFormat: Text.RichText
        wrapMode: TextEdit.WordWrap
        anchors {
            topMargin: header.height
            centerIn: parent.Center
            fill: parent
            margins: Theme.paddingLarge
        }
        text: "Copyright 2014 © by Aleš Katona. All rights reserved. Phone controls DBus code inspired by <a href='https://github.com/ejjoman/SailfishReboot'>SailfishReboot</a>"
        onLinkActivated: {
             Qt.openUrlExternally(link);
        }
    }

}
