import QtQuick 2.0
import Sailfish.Silica 1.0
import "./UsageHelper.js" as Usage

Canvas {
    id: usageGraph

    property string indicatorName : ""
    property int dataSource
    property int tickSource
    property int xAxisSize: 10

    onTickSourceChanged: {
        if ( Usage.history.length != xAxisSize ) {
            Usage.history = [];
            for ( var i = 0; i < xAxisSize; i++ ) {
                Usage.history.push(0);
            }
        }

        Usage.history.push(dataSource)
        Usage.history.shift(); // keep max of xAxisSize data

        requestPaint()
    }

    anchors {
        left: parent.left
        right: parent.right
        margins: Theme.paddingLarge
    }

    height: 200

    onPaint: {
        var stepX = width / (xAxisSize > 0 ? xAxisSize : 1)
        var stepY = height / 100
        var context = getContext("2d");

        context.fillStyle = Theme.secondaryHighlightColor
        context.strokeStyle = Theme.highlightColor

        context.beginPath()
        context.clearRect(0, 0, width, height)
        context.fill()

        context.beginPath()
        context.moveTo(0, height - stepY * Usage.history[0])
        for ( var i = 1; i < Usage.history.length; i++ ) {
            context.lineTo(stepX * (i + 1), height - stepY * Usage.history[i])
        }
        context.lineTo(stepX * xAxisSize, height)
        context.lineTo(0, height)
        context.lineTo(0, height - stepY * Usage.history[0])

        context.fill()
        context.stroke()
    }

    Label {
        id: usageLabel

        anchors.centerIn: parent
        color: Theme.highlightColor
        text: parent.indicatorName
        opacity: 0.7
    }
}
