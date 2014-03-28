/*
  Copyright (C) 2013 Jolla Ltd.
  Contact: Thomas Perl <thomas.perl@jollamobile.com>
  All rights reserved.

  You may use this file under the terms of BSD license as follows:

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Jolla Ltd nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {
    Column {        
        ProgressCircleBase {
            width: parent.parent.width / 1.2
            height: width
            anchors.horizontalCenter: parent.horizontalCenter
            value: proc.coverPage === 0 ? cpu.summaryValue / 100 : memory.summaryValue / 100
            borderWidth: 2
            progressColor: Theme.highlightColor

            Text {
                width: parent.width
                anchors.centerIn: parent
                color: Theme.highlightColor
                font.pixelSize: Theme.fontSizeHuge
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: (proc.coverPage === 0 ? cpu.summaryValue : memory.summaryValue) + '%'
            }
        }

        Label {
            anchors.margins: Theme.paddingLarge
            text: (proc.coverPage === 0) ? "CPU" : "Memory";
        }

        anchors.centerIn: parent
    }

    CoverActionList {
        id: coverAction

        CoverAction {
            iconSource: "../images/cover-image-cpu.png"
            onTriggered: proc.coverPage = 0
        }

        CoverAction {
            iconSource: "../images/cover-image-memory.png"
            onTriggered: proc.coverPage = 1
        }
    }
}


