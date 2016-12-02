import QtQuick 2.5

Rectangle {
    id:bootTop
    color:"transparent"
    property alias bootStatus:bootStatusText
    Text{
        id:displayText
        anchors.right:bootSettingsButton.left
        anchors.rightMargin:8
        anchors.top:parent.top
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        text:"Boot Settings"
        font.pixelSize:18
        font.underline: true
        font.bold: true
        color:"white"
    }


    Rectangle{
        color:"#38761d"
        border.color: "white"
        border.width: 2
        radius:2
        height: 36
        width: parent.width * .75
        anchors.bottom: parent.bottom
        anchors.right: bootSettingsButton.left
        anchors.rightMargin: 6
        Text{
            id:bootStatusText
            color:"white"
            text:"Write Default"
            anchors.fill: parent
            anchors.margins: 1
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 18
        }
    }


    /*RadioButton{
        id:preserveBootIni
        anchors.left:displayText.right
        anchors.leftMargin: 15
        anchors.top:parent.top
        anchors.margins: 6
        width:32
        height:32
        color:"transparent"
        radius:4
        border.color: "#00ff00"
        border.width: 2
        source:"/images/check.png"
    }
    Text{
        id:preserveText
        anchors.left:preserveBootIni.right
        anchors.top:preserveBootIni.top
        anchors.bottom: filenameContainer.top
        anchors.margins: 3
        color: "#11d011"
        font.pixelSize: 22
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text:"Preserve Existing"
    }*/

    //TODO: change file open button to boot settings (configure boot options)
    Button{
        id:bootSettingsButton
        anchors.bottom:parent.bottom
        anchors.right: parent.right
        anchors.rightMargin:6
        anchors.bottomMargin: 6
        height:42
        width:42
        radius:8
        color:"black"
        border.width: 0
        buttonImage:Image{
            anchors.centerIn: parent
            height:42
            width:42
            source:"/images/boot.png"
            fillMode: Image.PreserveAspectFit
            Component.onCompleted: parent = bootSettingsButton;
        }
        mouseArea.hoverEnabled: true
        mouseArea.onHoveredChanged: {
            if(mouseArea.containsMouse)
            {
                border.width = 2
                bootSettingsButton.border.color = "orange"
            }
            else
            {
                border.width = 0
            }
        }
    }
}
