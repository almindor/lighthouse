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
import "../components"

Page {
    id: page

    Column {
        PageHeader {
            title: process.selectedName
        }

        anchors.fill: parent

        Label {
            text: qsTr("CPU: ") + process.selectedCPUUsage + "%"
            anchors {
                horizontalCenter: parent.horizontalCenter
            }
        }

        Label {
            text: qsTr("Memory: ") + process.selectedMemoryUsage + "%"
            anchors {
                horizontalCenter: parent.horizontalCenter
            }
        }

        Button {
            text: qsTr("Kill")
            anchors {
                horizontalCenter: parent.horizontalCenter
            }

            onClicked: {
                if ( process.killSelected() === 0 ) {
                    pageStack.navigateBack(PageStackAction.Animated);
                } else {
                    applicationWindow.infoPopupRef.show("Info", "Permission denied", 2000, true)
                }
            }
        }
    }
}
