import QtQuick 2.5

Rectangle {
    id:deviceTop
    color:"transparent"
    border.color: "white"
    border.width: 2
    anchors.margins: 8
    height:parent.height
    Text{
        id:deviceText
        anchors.left:parent.left
        anchors.top:parent.top
        text:"Device"
        font.underline: true
        anchors.margins: 8
        font.pixelSize: 25
        font.bold: true
        color:"white"
    }
    Text{
        id:displayText
        anchors.right:parent.right
        anchors.top:parent.top
        text:"Display"
        font.underline: true
        anchors.margins: 8
        font.pixelSize: 25
        font.bold: true
        color:"white"

    }
}

