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
        visible: isPortrait
        width: page.width
        spacing: Theme.paddingLarge
        PageHeader {
            title: qsTr("CPU Usage")
        }

        ProgressCircleBase {
            width: parent.width / 2
            height: width
            anchors.horizontalCenter: parent.horizontalCenter
            value: cpu.summaryValue / 100
            borderWidth: 2
            progressColor: Theme.highlightColor

            DoubleIndicator {
              topVal: cpu.summaryValue
              topUnit: "%"
              botVal: cpu.temperature
              botUnit: "C°"
            }
        }

        Repeater {
            model: cpu

            ProgressBar {
                minimumValue: 0
                maximumValue: 100
                value: cpuUsage
                valueText: value + '%'
                label: qsTr("CPU") + (index + 1);
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }
            }
        }
    }
    PageHeader {
        visible: isLandscape
        title: qsTr("CPU Usage")
    }

    ProgressCircleBase {
        visible: isLandscape
        id: circle
        x: Theme.paddingLarge
        width: parent.width / 3.5
        height: width
        anchors.verticalCenter: parent.verticalCenter
        value: cpu.summaryValue / 100
        borderWidth: 2
        progressColor: Theme.highlightColor

        DoubleIndicator {
            topVal: cpu.summaryValue
            topUnit: "%"
            botVal: cpu.temperature
            botUnit: "C°"
        }
    }
    Column {
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: circle.right
        visible: isLandscape
        width: parent.width - circle.width
        Repeater {
            model: cpu

            ProgressBar {
                minimumValue: 0
                maximumValue: 100
                value: cpuUsage
                valueText: value + '%'
                label: qsTr("CPU") + (index + 1);
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }
            }
        }
    }
}
