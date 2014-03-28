import QtQuick 2.0

PathView {

    property real valueMain: 0.0
    property real valueSecondary: 0.0
    property color colorMain
    property color colorSecondary
    property real size: parent.width < parent.height ? parent.width : parent.height

    Path {
        startX: 100; startY: 0

        PathArc {
            x: 0; y: 100
            radiusX: 100; radiusY: 100
            useLargeArc: true
        }
    }
}
