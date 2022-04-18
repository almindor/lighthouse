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

    SilicaFlickable {
        anchors.fill: parent

        PullDownMenu {
            MenuItem {
                text: qsTr("About")
                onClicked: pageStack.push(Qt.resolvedUrl("About.qml"))
            }
            // impossible in sailjail
            MenuItem {
                text: qsTr("Device Controls")
                onClicked: pageStack.push(Qt.resolvedUrl("Phone.qml"))
            }
            MenuItem {
                text: qsTr("Settings")
                onClicked: pageStack.push(Qt.resolvedUrl("Settings.qml"))
            }
        }

        contentHeight: column.height

        Column {
            id: column

            width: page.width
            spacing: Theme.paddingLarge
            PageHeader {
                title: qsTr("Summary")
            }

            Row {
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    margins: Theme.paddingLarge
                }

                Label {
                    color: Theme.primaryColor
                    text: qsTr("Uptime: ")
                }

                Label {
                    color: Theme.highlightColor
                    text: monitor.uptime
                }
            }

            Button {
                text: qsTr("CPUs: ") + cpu.summaryValue + "%"
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    margins: Theme.paddingLarge
                }
                onClicked: pageStack.push(Qt.resolvedUrl("CPU.qml"))
            }


            Button {
                text: qsTr("Memory: ") + memory.summaryValue + "%"
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    margins: Theme.paddingLarge
                }
                onClicked: pageStack.push(Qt.resolvedUrl("Memory.qml"))
            }

            Button {
                text: qsTr("Processes: ") + process.summaryValue
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    margins: Theme.paddingLarge
                }
                onClicked: pageStack.push(Qt.resolvedUrl("Process.qml"))
            }

            Button {
                text: qsTr("Battery: ") + battery.summaryValue + "%"
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    margins: Theme.paddingLarge
                }
                onClicked: pageStack.push(Qt.resolvedUrl("Battery.qml"))
            }

        }
    }
}


