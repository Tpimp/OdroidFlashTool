import QtQuick 2.6

Rectangle {
    id: topRect
    property bool value:false
    property alias text:slideText
    property alias handle:switchHandle
    onValueChanged:{
        if(value)
            switchHandle.x = (topRect.width - switchHandle.width) + 10
        else
            switchHandle.x = -10
    }

    Text{
        id:slideText
        anchors.fill: parent
        anchors.margins: parent.width/12
        horizontalAlignment: value ? Text.AlignLeft:Text.AlignRight
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: parent.height * .36
    }
    Rectangle{
        id:switchHandle
        height:parent.height + 10
        width:parent.width * .38
        color:"grey"
        Drag.active: dragArea.drag.active
        anchors.verticalCenter: parent.verticalCenter
        radius:16
        border.color: "white"
        border.width: 1
        Row{
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            width:parent.width *.6
            height:parent.height * .8
            spacing:parent.width/6
            anchors.horizontalCenterOffset: spacing/2
            Rectangle{
                height:parent.height *.5
                color:"white"
                width:2
                anchors.verticalCenter: parent.verticalCenter
            }
            Rectangle{
                height:parent.height *.9
                color:"white"
                width:2
                anchors.verticalCenter: parent.verticalCenter
            }
            Rectangle{
                height:parent.height *.5
                color:"white"
                width:2
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        MouseArea{
            id:dragArea
            drag.target: switchHandle
            anchors.fill: parent
            drag.axis: Drag.XAxis
            drag.minimumX: -10
            drag.maximumX: topRect.width - switchHandle.width + 10
            onReleased: {
                if((switchHandle.x + (switchHandle.width/2)) >= (topRect.width/2))
                {
                    // over threshold
                    switchHandle.x = (topRect.width - switchHandle.width) + 10
                    value = true;
                }
                else
                {
                    switchHandle.x = - 10
                    value = false;
                }
            }
            hoverEnabled:true
            onHoveredChanged:
            {
                if(dragArea.containsMouse)
                    switchHandle.border.color = "#38761d"
                else
                    switchHandle.border.color = "white"
            }
        }
        Behavior on x{
            NumberAnimation{
                duration:350
            }
        }
    }
}
