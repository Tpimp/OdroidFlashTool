import QtQuick 2.5

Rectangle {
    id:progressTop
    signal progressComplete()
    signal progressStart()
    signal progressUpdated(int progress)
    property alias progressText:progressText.text
    property alias  verifyAfter:verifyFlag.checked
    color:"transparent"
    border.color: "white"
    border.width: 2
    Text{
        id:progressTitle
        anchors.left:parent.left
        anchors.top:parent.top
        anchors.topMargin: 8
        text:"Operation Status"
        font.underline: true
        font.pixelSize: 24
        font.bold: true
        anchors.leftMargin: 6
        width: 38
        color:"white"
    }
    Connections{
        target:ODF
        onAppendProcess:processView.appendItem(name)
        onRemoveProcessAt:processView.removeItem(index)
        onClearProcessList:processView.clearList()
        onInsertProcess:processView.insertItem(index,name)
    }

    ProcessView{
        id:processView
        anchors.horizontalCenter: progressBar.horizontalCenter
        height:36
        anchors.bottom: progressBar.top
        anchors.bottomMargin: 8
    }
    RadioButton{
        id:verifyFlag
        color:"transparent"
        radius:4
        border.color: "#00ff00"
        border.width: 2
        anchors.top:parent.top
        anchors.right:verifyText.left
        anchors.margins: 8
        anchors.topMargin: 16
        width:26
        height:26
        source:"/images/check.png"
        checked:AppSettings.mVerifyFlash
        onCheckedChanged: {AppSettings.setVerifyFlash(verifyFlag.checked);}
    }
    Text{
        id:verifyText
        anchors.verticalCenter: verifyFlag.verticalCenter
        anchors.right: parent.right
        anchors.margins: 8
        color: "#11d011"
        font.pixelSize: 18
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text:"Use Block Verify"
    }
    ProgressBar{
        id:progressBar
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin:8
        width:parent.width * .9
        height:32
        radius:8
        anchors.leftMargin: 8
        border.color:"#00ff00"
        color:"transparent"
        innerColor:"#6aa84f"
        innerBorder.color:"#38761d"
        onValueChanged: progressUpdated(value)
        onProgressFinished: {
            progessTop.progressComplete()
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
            text:"Ready To Write"
        }
    }
    Text{
        id:waitTime
        anchors.horizontalCenter: progressBar.horizontalCenter
        anchors.top:progressBar.bottom
        width:progressBar.width/2
        height: 16
        color:"white"
        horizontalAlignment: Text.AlignRight
        verticalAlignment: Text.AlignHCenter
        anchors.margins:1
        font.pixelSize: 14
        visible:false
        text: ""
    }
}
