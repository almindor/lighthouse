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
import Nemo.Notifications 1.0
import "../components"

Page {
    id: page
    allowedOrientations: Orientation.Portrait | Orientation.Landscape
                         | Orientation.LandscapeInverted

    Notification {
        id: notification
        appName: "lighthouse"
    }

    function banner(category, message) {
        notification.close()
        notification.previewBody = message
        notification.previewSummary = "lighthouse"
        notification.publish()
    }

    SilicaFlickable {
        anchors.fill: parent
        contentWidth: parent.width
        contentHeight: col.height

        VerticalScrollDecorator {
        }

        Column {
            id: col
            PageHeader {
                title: "Details"
            }

            width: page.width
            spacing: 10

            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.margins: Theme.paddingLarge
                text: process.selectedName
                visible: process.selectedName !== process.selectedProcName
            }

            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.margins: Theme.paddingLarge
                text: process.selectedProcName
                color: Theme.highlightColor
            }

            ProcIndicator {
                withName: false
                procName: process.selectedName
                cpuUse: process.selectedCPUUsage
                memUse: process.selectedMemoryUsage
            }

            UsageGraph {
                indicatorName: qsTr("CPU", "details CPU indicator name")
                dataSource: process.selectedCPUUsage
                tickSource: process.selectedTick
            }

            UsageGraph {
                indicatorName: qsTr("Memory", "details Memory indicator name")
                dataSource: process.selectedMemoryUsage
                tickSource: process.selectedTick
            }

            // impossible in sailjail
            Button {
                text: process.isKillable() ? qsTr("Kill") : qsTr("Permission denied")
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }

                enabled: process.isKillable()

                onClicked: {
                    if (process.killSelected() === 0) {
                        pageStack.navigateBack(PageStackAction.Animated)
                    } else {
                        banner("INFO", qsTr("Permission denied"))
                    }
                }
            }
        }
    }
}
