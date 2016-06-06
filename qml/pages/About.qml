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
    allowedOrientations: Orientation.Portrait | Orientation.Landscape
    | Orientation.LandscapeInverted
    property bool largeScreen: Screen.sizeCategory === Screen.Large ||
    Screen.sizeCategory === Screen.ExtraLarge

    SilicaFlickable {
        anchors.fill: parent
        contentWidth: parent.width
        contentHeight: col.height

        VerticalScrollDecorator {}

        Column {
            id: col
            width: page.width
            spacing: Theme.paddingLarge

            PageHeader {
                id: header
                title: qsTr("About")
            }

            Label {
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }
                wrapMode: Text.WordWrap
                color: Theme.primaryColor
                text: qsTr("Lighthouse") + " v1.6.0 " + qsTr("by Aleš Katona and Lavaux Gilles.")
            }

            Label {
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }
                wrapMode: Text.WordWrap
                color: Theme.primaryColor
                text: qsTr("Translators")
            }

            Text {
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }
                color: Theme.secondaryColor
                font.pointSize: largeScreen ? 30 : 16
                wrapMode: Text.WordWrap
                text: qsTr("Czech translation by <a href='https://www.transifex.com/accounts/profile/fri/'>fri</a>.")
                linkColor: Theme.secondaryHighlightColor
                onLinkActivated: {
                    Qt.openUrlExternally(link);
                }
            }

            Text {
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }
                color: Theme.secondaryColor
                font.pointSize: largeScreen ? 30 : 16
                wrapMode: Text.WordWrap
                text: qsTr("Russian translation by <a href='https://www.transifex.com/accounts/profile/lewa/'>Alexey (lewa)</a>.")
                linkColor: Theme.secondaryHighlightColor
                onLinkActivated: {
                    Qt.openUrlExternally(link);
                }
            }

            Text {
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }
                color: Theme.secondaryColor
                font.pointSize: largeScreen ? 30 : 16
                wrapMode: Text.WordWrap
                text: qsTr("Spanish translation by <a href='https://www.transifex.com/accounts/profile/hobarrera/'>Hugo Osvaldo Barrera (hobarrera)</a>.")
                linkColor: Theme.secondaryHighlightColor
                onLinkActivated: {
                    Qt.openUrlExternally(link);
                }
            }

            Text {
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }
                color: Theme.secondaryColor
                font.pointSize: largeScreen ? 30 : 16
                wrapMode: Text.WordWrap
                text: qsTr("Taiwanese translation by <a href='https://www.transifex.com/accounts/profile/hanhsuan/'>hanhsuan</a>.")
                linkColor: Theme.secondaryHighlightColor
                onLinkActivated: {
                    Qt.openUrlExternally(link);
                }
            }

            Text {
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }
                color: Theme.secondaryColor
                font.pointSize: largeScreen ? 30 : 16
                wrapMode: Text.WordWrap
                text: qsTr("Chinese translation by <a href='https://www.transifex.com/accounts/profile/TylerTemp/'>TylerTemp</a>.")
                linkColor: Theme.secondaryHighlightColor
                onLinkActivated: {
                    Qt.openUrlExternally(link);
                }
            }

            Text {
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }
                color: Theme.secondaryColor
                font.pointSize: largeScreen ? 30 : 16
                wrapMode: Text.WordWrap
                text: qsTr("French translation by <a href='https://www.transifex.com/accounts/profile/Nerfiaux/'>Guillaume ARIAUX (Nerfiaux)</a>.")
                linkColor: Theme.secondaryHighlightColor
                onLinkActivated: {
                    Qt.openUrlExternally(link);
                }
            }

            Text {
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }
                color: Theme.secondaryColor
                font.pointSize: largeScreen ? 30 : 16
                wrapMode: Text.WordWrap
                text: qsTr("Italian translation by <a href='https://www.transifex.com/accounts/profile/ghostofasmile/'>Francesco Vaccaro (ghostofasmile)</a>.")
                linkColor: Theme.secondaryHighlightColor
                onLinkActivated: {
                    Qt.openUrlExternally(link);
                }
            }

            Text {
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }
                color: Theme.secondaryColor
                font.pointSize: largeScreen ? 30 : 16
                wrapMode: Text.WordWrap
                text: qsTr("Swedish translation by <a href='https://www.transifex.com/accounts/profile/AsavarTzeth/'>Patrik Nilsson (AsavarTzeth)</a>.")
                linkColor: Theme.secondaryHighlightColor
                onLinkActivated: {
                    Qt.openUrlExternally(link);
                }
            }

            Text {
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }
                color: Theme.secondaryColor
                font.pointSize: largeScreen ? 30 : 16
                wrapMode: Text.WordWrap
                text: qsTr("Lighthouse is free software licensed under the GPL v3 available at <a href='https://github.com/almindor/lighthouse'>github</a>.\
                Device controls DBus code inspired by <a href='https://github.com/ejjoman/SailfishReboot'>SailfishReboot</a>.")
                linkColor: Theme.secondaryHighlightColor
                onLinkActivated: {
                    Qt.openUrlExternally(link);
                }
            }

        }
    }
}
