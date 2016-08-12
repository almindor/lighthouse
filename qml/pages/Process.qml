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
    allowedOrientations: Orientation.Portrait | Orientation.Landscape
                         | Orientation.LandscapeInverted
    onStatusChanged: {
        if ( status === PageStatus.Active ) {
            process.selectedPID = 0
            monitor.setProcessDetails( -1 ) // monitor all
        } else if ( status === PageStatus.Inactive ) {
            monitor.setProcessDetails( process.selectedPID ) // monitor pid or nothing if going back
        }
    }

    SilicaListView {
        id: listView
        header: PageHeader {
            title: process.applicationsOnly ? qsTr("Applications") : qsTr("Processes")
        }

        property int preSelectedPID : 0

        PullDownMenu {
            MenuItem {
                text: (process.applicationsOnly ? qsTr("Show Processes") : qsTr("Show Applications"))
                onClicked: process.nextApplicationsOnly()
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
        VerticalScrollDecorator {
            flickable: listView
        }

        delegate: ProcIndicator {
            procName: name
            cpuUse: cpuUsage
            memUse: memoryUsage

            onPressed: {
                listView.preSelectedPID = processID
            }

            onPressAndHold: {
                if ( process.selectedPID == 0 && listView.preSelectedPID > 0 ) {
                    process.selectedPID = listView.preSelectedPID
                }

                pageStack.push(Qt.resolvedUrl("Details.qml"))
            }
        }
    }
}
