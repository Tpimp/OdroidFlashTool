import QtQuick 2.5

Rectangle {
    id:bootTop
    color:"transparent"
    border.color: "white"
    border.width: 2
    anchors.margins: 8
    Text{
        id:displayText
        anchors.left:parent.left
        anchors.top:parent.top
        anchors.topMargin:8
        text:"Boot File"
        font.underline: true
        anchors.leftMargin: 8
        font.pixelSize:22
        font.bold: true
        color:"white"
    }

    Rectangle{
        id:filenameContainer
        radius:8
        width: parent.width *.8
        height: 36
        border.color: "#6aa84f"
        border.width: 2
        anchors.top:displayText.bottom
        anchors.topMargin: 8
        anchors.left:parent.left
        anchors.leftMargin: 8
        anchors.right: fileOpenButton.left
        anchors.rightMargin: 4
        clip:true
        TextInput{
            id: inputFilePath
            anchors.fill: parent
            anchors.leftMargin: 4
            anchors.rightMargin: 4
            horizontalAlignment: TextInput.AlignLeft
            verticalAlignment: TextInput.AlignVCenter
            text: ""
            font.pixelSize: (width/text.length+ 8) < parent.height *.62 ? (width/text.length+ 8):parent.height *.62
        }
    }
    RadioButton{
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
    }

    Button{
        id:fileOpenButton
        anchors.verticalCenter: filenameContainer.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin:8
        height:36
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
}
