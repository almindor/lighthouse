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
import "../components"

CoverBackground {

    function getTopVal(cp) {
        switch ( cp ) {
            case 0: return cpu.summaryValue
            case 1: return memory.summaryValue
            case 2: return battery.summaryValue
        }

        return -1
    }

    function getBotVal(cp) {
        switch ( cp ) {
            case 0: return cpu.temperature
            case 1: return Math.round(memory.free / 1000)
            case 2: return battery.statusShort
        }

        return ""
    }

    function getBotUnit(cp) {
        switch ( cp ) {
            case 0: return "C°"
            case 1: return "MB"
            case 2: return ""
        }

        return ""
    }

    Image {
        id: bgimg
        source: "../images/cover.png"
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width
        height: sourceSize.height * width / sourceSize.width
    }

    Label {
        anchors.top: parent.top
        anchors.margins: Theme.paddingLarge
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: Theme.fontSizeLarge
        horizontalAlignment: Text.AlignHCenter
        color: Theme.highlightColor
        text: monitor.coverLabel
    }

    ProgressCircleBase {
        width: parent.parent.width / 1.2
        height: width
        anchors {
            horizontalCenter: parent.horizontalCenter
            centerIn: parent
        }
        value: getTopVal(monitor.coverPage) / 100.0
        borderWidth: 2
        progressColor: Theme.highlightColor

        DoubleIndicator {
            topVal: getTopVal(monitor.coverPage)
            topUnit: "%"
            botVal: getBotVal(monitor.coverPage)
            botUnit: getBotUnit(monitor.coverPage)
        }
    }

    CoverActionList {
        id: coverAction

        CoverAction {
            iconSource: "../images/cover-image-" + monitor.coverImageLeft + ".png"
            onTriggered: monitor.coverPage--
        }

        CoverAction {
            iconSource: "../images/cover-image-" + monitor.coverImageRight + ".png"
            onTriggered: monitor.coverPage++
        }        
    }
}


