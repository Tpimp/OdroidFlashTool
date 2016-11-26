import QtQuick 2.5

Rectangle {
    id:progressTop
    signal progressComplete()
    signal progressStart()
    signal progressUpdated(int progress)
    property alias progressText:progressText.text
    color:"transparent"
    border.color: "white"
    border.width: 2
    Text{
        id:progressTitle
        anchors.left:parent.left
        anchors.top:parent.top
        anchors.topMargin: 6
        text:"Progress"
        font.underline: true
        font.pixelSize: 20
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
        height:30
        anchors.bottom: progressBar.top
        anchors.bottomMargin: 4
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
