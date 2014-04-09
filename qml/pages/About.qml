/*
    Copyright (C) 2014 Ales Katona.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
