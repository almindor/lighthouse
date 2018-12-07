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

    PageHeader {
        id: headerP
        title: qsTr("CPU Usage")
    }

    ProgressCircleBase {
        id: circleP
        anchors.top: headerP.bottom
        width: (isLandscape ? parent.height : parent.width) / 2
        height: width
        anchors.horizontalCenter: (isPortrait ? parent.horizontalCenter : undefined)
        anchors.verticalCenter: (isLandscape ? parent.verticalCenter : undefined)
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

    SilicaListView {
        id: listViewP
        anchors {
            top: (isLandscape ? headerP.bottom : circleP.bottom)
            bottom: parent.bottom
            left: (isLandscape ? circleP.right : parent.left)
            right: parent.right
            leftMargin: (isLandscape ? 0 : Theme.horizontalPageMargin)
            rightMargin: (isLandscape ? Theme.paddingLarge : Theme.horizontalPageMargin)
        }

        model: cpu
        VerticalScrollDecorator {
            flickable: listViewP
        }

        clip: true
        delegate: ProgressBar {
            width: ListView.view.width
            minimumValue: 0
            maximumValue: 100
            value: cpuUsage
            valueText: value + '%'
            label: qsTr("CPU") + (index + 1);
        }
    }
}
