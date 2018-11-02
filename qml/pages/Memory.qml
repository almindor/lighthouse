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

    function kiBToMiB(size){
        if (size < 100){
            return Math.round(size*10 / 1024) / 10;
        }else{
            return Math.round(size / 1024);
        }
    }

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
                topVal: kiBToMiB(memory.total - memory.free) + " "
                topUnit: "MiB"
                botVal: kiBToMiB(memory.total) + " "
                botUnit: "MiB"
            }
        }

        ProgressBar {
            minimumValue: 0
            maximumValue: 100
            value: (100 - memory.summaryValue)
            label: qsTr("%1 MiB free out of %2 MiB total").arg(kiBToMiB(memory.free)).arg(kiBToMiB(memory.total))
            anchors {
                left: parent.left
                right: parent.right
                margins: Theme.paddingLarge
            }
        }
        SectionHeader{
            text: qsTr("zRam swap")
        }

        ProgressBar {
            minimumValue: 0
            maximumValue: zram.diskSize
            value: zram.data
            label: qsTr("%1 MiB from %2 MiB used (compression %3 %)")
                        .arg(kiBToMiB(zram.data))
                        .arg(kiBToMiB(zram.diskSize))
                        .arg(zram.data == 0 ? "-" : Math.round((1-(zram.compressed / zram.data)) * 100))

            anchors {
                left: parent.left
                right: parent.right
                margins: Theme.paddingLarge
            }
        }
    }
}
