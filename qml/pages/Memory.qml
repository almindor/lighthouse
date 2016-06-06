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
    Column {
        width: page.width
        spacing: Theme.paddingLarge
        PageHeader {
            title: qsTr("Memory Usage")
        }

        ProgressCircleBase {
            width: isPortrait ? parent.width /2 : parent.width / 3.5
            height: width
            anchors.horizontalCenter: parent.horizontalCenter
            value: 1 - (memory.free / memory.total)
            borderWidth: 2
            progressColor: Theme.highlightColor

            DoubleIndicator {
                topVal: (memory.total - memory.free)
                topUnit: "kB"
                botVal: memory.total
                botUnit: "kB"
            }
        }

        ProgressBar {
            minimumValue: 0
            maximumValue: 100
            value: (100 - memory.summaryValue)
            label: memory.free + qsTr(" kB free out of ") + memory.total + qsTr(" kB total")
            anchors {
                left: parent.left
                right: parent.right
                margins: Theme.paddingLarge
            }
        }
    }
}
