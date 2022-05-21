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


Page {
    id: page
    allowedOrientations: Orientation.Portrait | Orientation.Landscape
                         | Orientation.LandscapeInverted

    PageHeader {
        id: pageHeader
        anchors.top: parent.top
        title: qsTr("Settings")
    }

    Component.onDestruction: notification.close()

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

    Slider {
        id: slider
        minimumValue: 1
        maximumValue: 10
        stepSize: 1
        value: monitor.interval
        valueText: value + 's'
        label: qsTr("Update interval")
        onValueChanged: monitor.interval = value
        anchors {
            top: pageHeader.bottom
            left: parent.left
            right: parent.right
            margins: Theme.paddingLarge
        }
    }

    PageHeader {
        id: langHeader
        anchors.top: slider.bottom
        title: qsTr("Languages")
    }

    SilicaListView {
        id: languageList
        spacing: 2
        anchors {
            top: langHeader.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            margins: Theme.paddingLarge
        }

        VerticalScrollDecorator {}

        model: languages

        delegate: BackgroundItem {
            id: languageDelegate
            anchors {
                left: parent.left
                right: parent.right
                margins: Theme.paddingSmall
            }

            Label {
                anchors.centerIn: parent
                text: language + " [" + country + "]"
            }

            onClicked: {
                languages.selectLanguage(index)
                banner("INFO", qsTr("App Restart Required"))
            }
        }
    }

}
