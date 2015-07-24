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
    onStatusChanged: monitor.setProcessDetails( status !== 0 )

    SilicaListView {
        id: listView
        header: PageHeader {
            title: process.applicationsOnly ? qsTr("Applications") : qsTr("Processes")
        }

        PullDownMenu {
            MenuItem {
                text: (process.applicationsOnly ? qsTr("Show Processes") : qsTr("Show Applications"))
                onClicked: process.nextApplicationsOnly();
            }

            MenuItem {
                text: qsTr("Sort by Name")
                onClicked: process.setSortBy(2)
                visible: (process.sortBy !== 2)
            }

            MenuItem {
                text: qsTr("Sort by Memory Usage")
                onClicked: process.setSortBy(1)
                visible: (process.sortBy !== 1)
            }

            MenuItem {
                text: qsTr("Sort by CPU Usage")
                onClicked: process.setSortBy(0)
                visible: (process.sortBy !== 0)
            }
        }

        anchors.fill: parent
        spacing: 2
        model: process
        VerticalScrollDecorator {}

        delegate: Indicator {
            onPressAndHold: {
                if ( process.selectedPID == 0 ) {
                    processDock.show()
                    process.selectedPID = processID
                }
            }

            onPressed: {
                if ( process.selectedPID > 0 ) {
                    processDock.hide()
                    process.selectedPID = 0
                }
            }

            onPressedChanged: {

            }
        }

        OpaquePanel {
            id: processDock

            width: parent.width
            height: parent.height / 2

            dock: Dock.Bottom

            Column {
                anchors.centerIn: parent
                Text {
                    text: "PID: " + process.selectedPID
                }
                Button {
                    text: qsTr("Kill")
                }
            }
        }
    }
}
