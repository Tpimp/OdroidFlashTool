import QtQuick 2.5

Rectangle {
    id:outerRect
    property alias innerColor: innerRect.color
    property alias innerBorder: innerRect.border
    property real   value:0
    property real   minimumValue:0
    property real   maximumValue:100
    signal  progressFinished()
    onValueChanged: {
        innerRect.width = (outerRect.width - (outerRect.border.width * 2)) *(value/maximumValue);
        if(value >= maximumValue)
        {
            innerRect.width = (outerRect.width - (outerRect.border.width * 2)) - innerRect.anchors.margins
            value = maximumValue
            outerRect.progressFinished();
        }
    }

    Rectangle{
        id:innerRect
        radius:outerRect.radius
        anchors.left: parent.left
        anchors.top:parent.top
        anchors.bottom:parent.bottom
        anchors.margins: 2
        width:0
    }
}
