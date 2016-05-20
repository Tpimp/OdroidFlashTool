import QtQuick 2.6

Rectangle {
    Text{
        id:progressTitle
        anchors.left:deviceView.left
        anchors.top:deviceView.bottom
        text:"Progress"
        font.underline: true
        anchors.topMargin: 8
        font.pixelSize: 25
        font.bold: true
        width: parent.width/5
        color:"white"
    }
    ProgressBar{
        id:progressBar
        anchors.left: progressTitle.right
        anchors.right: deviceView.right
        anchors.top: deviceView.bottom
        anchors.topMargin:8
        height:30
        radius:8
        anchors.leftMargin: 8
        border.color:"#00ff00"
        color:"transparent"
        innerColor:"#6aa84f"
        innerBorder.color:"#38761d"
        onProgressFinished: {
            progressBar.value = 0;
            //progressTimer.stop()
        }
        Text{
            id:progressText
            anchors.fill:parent
            anchors.margins: 6
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color:"white"
            font.pixelSize: height
            text:"Writing Image to Drive Z:"
        }
    }
    Button{
        id:flashButton
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 8
        anchors.bottom:parent.bottom
        buttonText:Text{
            anchors.fill: parent
            text:  "Write Image"
            color:"white"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: parent.height * .55
            Component.onCompleted: parent = flashButton
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
                flashButton.color = "#999999"
                flashButton.border.color = "#00ff00"
            }
            else
            {
                flashButton.color = "#6aa84f"
                flashButton.border.color = "#999999"
            }
        }
    }
    ImageFileView{
        id:imageView
        height:parent.height *.42
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        onFlashModeChanged: {
            if(flashMode)
            {
                flashButton.buttonText.text = "Write Image"
                progressText.text = "Writing Image to Drive Z:"
                deviceView.setWriteMode()
            }
            else
            {
                flashButton.buttonText.text = "Read Image"
                progressText.text = "Reading Image from Drive Z:"
                deviceView.setReadMode()
            }
        }
    }
    DeviceView{
        id:deviceView
        height:parent.height *.3
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: imageView.bottom
    }

    Timer{
        id:progressTimer
        running: false
        interval:50
        repeat:true
        onTriggered: {
            progressBar.value++;
        }
    }
    Component.onCompleted: {
        progressTimer.start();
    }
}
