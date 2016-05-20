import QtQuick 2.5

Rectangle {
    id:deviceTop
    color:"transparent"
    border.color: "white"
    border.width: 2
    anchors.margins: 8
    height:parent.height
    property alias  verifyAfter:verifyFlag.checked
    function setWriteMode()
    {
        verifyText.text = "Verify<br>After Flash";
        displayDrop.visible = true;
        displayText.visible = true;
        configureBoot.visible = true;
    }
    function setReadMode()
    {
        verifyText.text = "Verify<br>After Read";
        displayDrop.visible = false;
        displayText.visible = false;
        configureBoot.visible = false;
    }

    Button{
        id:configureBoot
        anchors.right:parent.right
        anchors.margins: 8
        anchors.bottom:parent.bottom
        buttonText:Text{
            anchors.fill: parent
            text:"Configure boot.ini"
            color:"white"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: parent.height * .35
            Component.onCompleted: parent = configureBoot
        }
        color:"#6aa84f"
        border.color: "#b7b7b7"
        border.width: 1
        radius:12
        height:42
        width: parent.width *.38
        mouseArea.hoverEnabled: true
        mouseArea.onHoveredChanged: {
            if(mouseArea.containsMouse)
            {
                configureBoot.color = "#999999"
                configureBoot.border.color = "#00ff00"
            }
            else
            {
                configureBoot.color = "#6aa84f"
                configureBoot.border.color = "#999999"
            }
        }
    }

    Text{
        id:deviceText
        anchors.left:parent.left
        anchors.top:parent.top
        anchors.leftMargin: 8
        text:"Device"
        font.underline: true
        anchors.margins: 2
        font.pixelSize: 25
        font.bold: true
        color:"white"
    }
    RadioButton{
        id:verifyFlag
        color:"transparent"
        radius:4
        border.color: "#00ff00"
        border.width: 2
        anchors.left:parent.left
        anchors.bottom:parent.bottom
        anchors.margins: 12
        width:30
        height:30
        source:"/images/check.png"
    }
    Text{
        id:verifyText
        anchors.verticalCenter: verifyFlag.verticalCenter
        anchors.left: verifyFlag.right
        anchors.margins: 8
        color: "#11d011"
        font.pixelSize: 18
        font.bold: true
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text:"Verify<br>After Flash"
    }
    DropList{
        id:driveDrop
        anchors.top:deviceText.bottom
        anchors.topMargin: 8
        anchors.left:parent.left
        anchors.leftMargin: 24
        color:"#93c47d"
        width:parent.width * .26
        height:36
        radius:6
        border.color: "white"
        border.width: 2
        dropImage.source:"images/arrow_down.png"
        dropHandle.color: "#6aa84f"
        dropHandle.border.color:"#274e13"
        dropHandle.border.width: 2


        Component.onCompleted: {
            driveDrop.addItemToList("Drive:Z","Z")
            driveDrop.addItemToList("Drive:D","D")
            driveDrop.addItemToList("Drive:A","A")
            driveDrop.addItemToList("Drive:Z","Z")
            driveDrop.addItemToList("Drive:D","D")
            driveDrop.addItemToList("Drive:A","A")
            driveDrop.addItemToList("Drive:Z","Z")
            driveDrop.addItemToList("Drive:D","D")
            driveDrop.addItemToList("Drive:A","A")
            driveDrop.addItemToList("Drive:Z","Z")
            driveDrop.addItemToList("Drive:D","D")
            driveDrop.addItemToList("Drive:A","A")
        }
    }

    Text{
        id:displayText
        anchors.right:parent.right
        anchors.top:parent.top
        text:"Display"
        font.underline: true
        anchors.margins: 2
        anchors.rightMargin: 8
        font.pixelSize: 25
        font.bold: true
        color:"white"

    }
    DropList{
        id:displayDrop
        anchors.top:displayText.bottom
        anchors.topMargin: 8
        anchors.right:parent.right
        anchors.rightMargin: 24
        color:"#93c47d"
        width:parent.width * .44
        height:36
        radius:6
        border.color: "white"
        border.width: 2
        dropImage.source:"images/arrow_down.png"
        dropHandle.color: "#6aa84f"
        dropHandle.border.color:"#274e13"
        dropHandle.border.width: 2
        dropText.font.pixelSize: 18
        Component.onCompleted: {
            displayDrop.addItemToList("1080p - 1920x1080",Qt.size(1920,1080))
            displayDrop.addItemToList("720p - 1280x720",Qt.size(1280,720))
            displayDrop.addItemToList("480p - 800x480",Qt.size(800,480))
        }
    }


}

