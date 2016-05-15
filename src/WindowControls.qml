import QtQuick 2.6

Rectangle {
    color:"transparent"
    // Define signal events
    signal minimizeClicked()
    signal maximizeClicked()
    signal closeClicked()
    property color buttonColor:"darkgrey"
    property color innerColor:"white"
    property color hoverColor:"green"
    property alias minimizeButton:minimizeButton
    property alias maximizeButton:maximizeButton
    property alias closeButton:closeButton
    Row{
        anchors.top:parent.top
        anchors.bottom:parent.bottom
        anchors.right:parent.right
        anchors.margins: 3
        spacing:3
        Button{
            id:minimizeButton
            height: parent.height
            width:height
            mouseArea.hoverEnabled: true
            color:buttonColor
            border.width: 1
            radius:2
            Rectangle{
                id:miniRect
                color:"transparent"
                border.color: innerColor
                border.width: 2
                height:6
                width:parent.width * .9
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.margins: 4
            }
            mouseArea.preventStealing:true
            mouseArea.onHoveredChanged: {
                if(mouseArea.containsMouse)
                {
                    miniRect.border.color = hoverColor
                }
                else
                {
                    miniRect.border.color = innerColor
                }
            }
        }
        Button{
            id:maximizeButton
            height: parent.height
            width:height
            mouseArea.hoverEnabled: true
            color:buttonColor
            border.width: 1
            radius:2
            Rectangle{
                id:maxiRect
                color:"transparent"
                border.color: innerColor
                border.width: 2
                anchors.fill: parent
                anchors.margins: 4
                anchors.topMargin:6
                anchors.bottomMargin:4
            }
            mouseArea.preventStealing:true
            mouseArea.onHoveredChanged: {
                if(mouseArea.containsMouse)
                {
                    maxiRect.border.color = hoverColor
                }
                else
                {
                    maxiRect.border.color = innerColor
                }
            }
        }
        Button{
            id:closeButton
            height: parent.height
            width:height
            mouseArea.hoverEnabled: true
            color:buttonColor
            border.width: 1
            radius:2
            Rectangle{
                id:closeRect
                color:"transparent"
                border.color: innerColor
                border.width: 3
                height:parent.height *.8
                width:parent.width * .8
                radius:width/2
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: 2
                Rectangle{
                    color:parent.border.color
                    width:parent.width * .9
                    height:3
                    rotation:45
                    anchors.centerIn: parent
                }
                Rectangle{
                    width:parent.width * .9
                    height:3
                    rotation:-45
                    color:parent.border.color
                    anchors.centerIn: parent
                }

            }
            mouseArea.preventStealing:true
            mouseArea.onHoveredChanged: {
                if(mouseArea.containsMouse)
                {
                    closeRect.border.color = hoverColor
                }
                else
                {
                    closeRect.border.color = innerColor
                }
            }
        }
    }
    Component.onCompleted:{
        minimizeButton.mouseArea.clicked.connect(minimizeClicked)
        maximizeButton.mouseArea.clicked.connect(maximizeClicked)
        closeButton.mouseArea.clicked.connect(closeClicked)
    }
}
