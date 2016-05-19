import QtQuick 2.5

Rectangle{
    property bool flashMode: true // Write == true, Read == false
    id:imageBox
    color:"transparent"
    border.color: "white"
    border.width: 2
    anchors.margins: 8
    height:parent.height
    Text{
        id:imageFileText
        anchors.left:parent.left
        anchors.top:parent.top
        text:"Image File"
        font.underline: true
        anchors.margins: 8
        font.pixelSize: 25
        font.bold: true
        color:"white"

    }

    SlideSwitch{
        id:readWriteSwitch
        anchors.top: parent.top
        anchors.left:imageFileText.right
        anchors.margins: 12
        height: 40
        width: 160
        color:"#00ff00"
        border.color: "white"
        border.width: 2
        radius:8
        onValueChanged:{
            if(value)
            {
                text.text = "WRITE MODE"
            }
            else
            {
                text.text = "READ MODE"
            }
        }
        Component.onCompleted: {
            value = flashMode;
        }
    }

    Rectangle{
        id:filenameContainer
        radius:8
        width: parent.width *.75
        height: 40
        border.color: "#6aa84f"
        border.width: 2
        anchors.top:readWriteSwitch.bottom
        anchors.topMargin: 12
        anchors.left:parent.left
        anchors.leftMargin: 24
        TextInput{
            anchors.fill: parent
            anchors.leftMargin: 4
            horizontalAlignment: TextInput.AlignLeft
            verticalAlignment: TextInput.AlignVCenter
            text: ""
            font.pixelSize: parent.height *.65
        }
    }
    Button{
        id:fileOpenButton
        anchors.bottom: filenameContainer.bottom
        anchors.left: filenameContainer.right
        anchors.leftMargin:8
        height:filenameContainer.height * 1.8
        width:height
        radius:4
        color:"#6aa84f"
        border.color: "#999999"
        border.width: 2
        buttonImage:Image{
            anchors.fill: parent
            anchors.margins:4
            source:"/images/folder.png"
            fillMode: Image.PreserveAspectFit
            smooth:true
            Component.onCompleted: parent = fileOpenButton;
        }
        mouseArea.hoverEnabled: true
        mouseArea.onHoveredChanged: {
            if(mouseArea.containsMouse)
            {
                fileOpenButton.color = "#999999"
                fileOpenButton.border.color = "#00ff00"
            }
            else
            {
                fileOpenButton.color = "#6aa84f"
                fileOpenButton.border.color = "#999999"
            }
        }
    }
    RadioButton{
        id:decompressFlag
        color:"transparent"
        radius:4
        border.color: "#00ff00"
        border.width: 2
        anchors.left:parent.left
        anchors.top:filenameContainer.bottom
        anchors.margins: 20
        width:30
        height:30
        source:"/images/check.png"
    }
    Text{
        id:decompressText
        anchors.verticalCenter: decompressFlag.verticalCenter
        anchors.left: decompressFlag.right
        anchors.margins: 8
        color: "#11d011"
        font.pixelSize: 18
        font.bold: true
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text:"Decompress<br>Before Flash"
    }

    RadioButton{
        id:randomGuidFlag
        color:"transparent"
        radius:4
        border.color: "#00ff00"
        border.width: 2
        anchors.left:decompressText.right
        anchors.verticalCenter:decompressFlag.verticalCenter
        anchors.margins: 20
        width:30
        height:30
        source:"/images/check.png"

    }
    Text{
        id:assignRandomText
        anchors.verticalCenter: randomGuidFlag.verticalCenter
        anchors.left: randomGuidFlag.right
        anchors.margins: 8
        color: "#11d011"
        font.pixelSize: 18
        font.bold: true
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text:"Assign Random<br>GUIDs"
    }
    Button{
        id:guidButton
        anchors.left:assignRandomText.right
        anchors.margins: 12
        anchors.bottom:parent.bottom
        buttonText:Text{
            anchors.fill: parent
            text:"Set GUID Table"
            color:"white"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: parent.height * .35
            Component.onCompleted: parent = guidButton
        }
        color:"#6aa84f"
        border.color: "#b7b7b7"
        border.width: 1
        radius:12
        height:randomGuidFlag.height * 1.65
        width: parent.width *.28
        mouseArea.hoverEnabled: true
        mouseArea.onHoveredChanged: {
            if(mouseArea.containsMouse)
            {
                guidButton.color = "#999999"
                guidButton.border.color = "#00ff00"
            }
            else
            {
                guidButton.color = "#6aa84f"
                guidButton.border.color = "#999999"
            }
        }
    }

}
