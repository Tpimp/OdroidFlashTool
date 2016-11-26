import QtQuick 2.5

Rectangle{
    property bool flashMode: true // Write == true, Read == false
    property alias shouldCompress:compressFlag.checked
    property alias compressCheck:compressFlag
    signal fileBrowserOpen()
    property alias filePath:inputFilePath.text
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
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 12
        height: 40
        width: 160
        color:"#00ff00"
        border.color: "white"
        border.width: 2
        radius:8
        onValueChanged:{
            flashMode = value;
            AppSettings.setFlashMode(flashMode);
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
        anchors.right:fileOpenButton.left
        anchors.rightMargin: 2
        height: 36
        border.color: "#6aa84f"
        border.width: 2
        anchors.top:readWriteSwitch.bottom
        anchors.topMargin: 12
        anchors.left:parent.left
        anchors.leftMargin: 8
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
    Button{
        id:fileOpenButton
        anchors.bottom: filenameContainer.bottom
        anchors.rightMargin:8
        anchors.right:parent.right
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

    RadioButton{
        id:compressFlag
        color:"transparent"
        radius:4
        border.color: "#00ff00"
        border.width: 2
        anchors.left:readWriteSwitch.right
        anchors.verticalCenter: readWriteSwitch.verticalCenter
        anchors.margins: 26
        width:32
        height:32
        source:"/images/check.png"
        checked: false
        onCheckedChanged:AppSettings.setUseCompression(compressFlag.checked);
    }
    Text{
        id:compressText
        anchors.verticalCenter: compressFlag.verticalCenter
        anchors.left: compressFlag.right
        anchors.margins: 8
        color: "#11d011"
        font.pixelSize: 18
        font.bold: true
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text:flashMode ? "Decompress<br>Before Write":"Compress<br>After Read"
    }

//    RadioButton{
//        id:randomGuidFlag
//        color:"transparent"
//        radius:4
//        visible:flashMode
//        border.color:enabled? "#00ff00":"lightgrey"
//        border.width: 2
//        anchors.left:compressText.right
//        anchors.verticalCenter:compressText.verticalCenter
//        anchors.margins: 20
//        width:24
//        height:24
//        source:"/images/check.png"
//        enabled:false

//    }
//    Text{
//        id:assignRandomText
//        anchors.verticalCenter: randomGuidFlag.verticalCenter
//        anchors.left: randomGuidFlag.right
//        anchors.margins: 8
//        visible:flashMode
//        color: enabled? "#11d011":"lightgrey"
//        font.pixelSize: 18
//        font.bold: true
//        horizontalAlignment: Text.AlignHCenter
//        verticalAlignment: Text.AlignVCenter
//        enabled:false
//        text:"Assign Random<br>GUIDs"
//    }
//    Button{
//        id:guidButton
//        visible:flashMode
//        anchors.left:assignRandomText.right
//        anchors.margins: 12
//        anchors.bottom:parent.bottom
//        buttonText:Text{
//            anchors.fill: parent
//            text:"Set GUID Table"
//            color:"white"
//            horizontalAlignment: Text.AlignHCenter
//            verticalAlignment: Text.AlignVCenter
//            font.pixelSize: parent.height * .35
//            Component.onCompleted: parent = guidButton
//        }
//        color: "#999999"
//        border.color: "black"
//        border.width: 1
//        radius:12
//        height:randomGuidFlag.height * 1.65
//        width: parent.width *.28
//        mouseArea.hoverEnabled: true
//        enabled:false
//        mouseArea.onHoveredChanged: {
//            if(mouseArea.containsMouse)
//            {
//                guidButton.color = "#999999"
//                guidButton.border.color = "#00ff00"
//            }
//            else
//            {
//                guidButton.color = "#6aa84f"
//                guidButton.border.color = "#999999"
//            }
//        }
//    }

    Component.onCompleted: {
        fileOpenButton.mouseArea.clicked.connect(imageBox.fileBrowserOpen);
    }
}
