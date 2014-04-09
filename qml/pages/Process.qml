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

    SilicaListView {
        id: listView
        header: PageHeader {
            title: "Processes"
        }

        PullDownMenu {
            MenuItem {
                text: "Sort by: " + process.sortBy
                onClicked: process.nextSortBy()
            }

            onActiveChanged: {
                process.selectPID(0); // deselect and unpause
            }
        }

        anchors.fill: parent
        spacing: 2
        model: process
        VerticalScrollDecorator {}

        delegate: BackgroundItem {
            id: myListItem
            anchors {
                left: parent.left
                right: parent.right
                margins: Theme.paddingLarge
            }
            height: selected ? greyBar.height + killMenu.height : greyBar.height
            Label {
                anchors {
                    left: parent.left
                }

                id: itemLabel
                text: name
            }

            Text {
                id: cpuLabel
                text: "cpu: " + cpuUsage + "%"
                color: Theme.highlightColor
                font.pointSize: 12
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    top: parent.top
                }
            }

            Text {
                id: memLabel
                text: "mem: " + memoryUsage + "%"
                color: Theme.secondaryHighlightColor
                font.pointSize: 12
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    top: parent.top
                    topMargin: cpuLabel.height + 1
                }
            }

            Rectangle {
                id: greyBar
                anchors {
                    right: parent.right
                    top: parent.top
                }

                opacity: 0.5
                width: 200
                height: cpuLabel.height + memLabel.height + 1
                color: "dimgrey"
            }

            Rectangle {
                id: cpuBar
                anchors {
                    right: parent.right
                    top: parent.top
                }

                width: cpuUsage * 2
                height: cpuLabel.height
                color: Theme.highlightColor
            }

            Rectangle {
                id: memBar
                anchors {
                    right: parent.right
                    top: parent.top
                    topMargin: cpuBar.height + 1
                }

                width: memoryUsage * 2
                height: memLabel.height
                color: Theme.secondaryHighlightColor
            }

            onPressAndHold: {
                if ( process.isKillable(processID) ) {
                    process.selectPID(processID) // just in case it somehow sliped since onPressed
                    killMenu.show(myListItem)
                } else {
                    applicationWindow.infoPopupRef.show("Info", "Permission denied", 2000, true)
                }
            }

            onPressed: {
                process.selectPID(processID) // pauses the list updates
            }

            onReleased: {
                if ( !killMenu.active ) {
                    process.selectPID(0) // unpauses
                }
            }
        }

        ContextMenu {
            id: killMenu
            MenuItem {
                text: "Kill"
                onClicked: {
                    process.killSelectedProcess()
                }
            }

            onClosed: {
                process.selectPID(0) // deselect/depause
            }
        }
    }
}
