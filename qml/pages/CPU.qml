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
}
