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


    Column {
        width: page.width
        spacing: Theme.paddingLarge
        PageHeader {
            title: "Battery Usage"
        }

        ProgressCircleBase {
            width: parent.width / 2
            height: width
            anchors.horizontalCenter: parent.horizontalCenter
            value: battery.level/100.1
            borderWidth: 2
            progressColor: Theme.highlightColor

            Column {
                width: parent.width * 0.7
                anchors.centerIn: parent

                Text {
                    width: parent.width
                    color: Theme.highlightColor
                    //opacity: 0.9
                    font.pixelSize: Theme.fontSizeMedium
                    horizontalAlignment: Text.AlignHCenter
                    text: battery.status
                }
                Rectangle {
                    color: Theme.highlightColor
                    width: parent.width
                    height: 2
                    opacity: 0.4
                }
                Text {
                    width: parent.width
                    color: Theme.highlightColor
                    //opacity: 0.9
                    font.pixelSize: Theme.fontSizeMedium
                    horizontalAlignment: Text.AlignHCenter
                    text: battery.level
                }
            }

        }

        Text {
            width: parent.width
            color: Theme.highlightColor
            font.pixelSize: Theme.fontSizeSmall
            horizontalAlignment: Text.AlignHCenter
            text: "Health: " + battery.health
        }

        Text {
            width: parent.width
            color: Theme.highlightColor
            font.pixelSize: Theme.fontSizeSmall
            horizontalAlignment: Text.AlignHCenter
            text: "Technology: " + battery.technology
        }
    }
}

