import QtQuick 2.6
import QtQuick.Window 2.2
import QtWinExtras 1.0

Window {
    id:mainWindow
    visible: true
    height: 480
    width: 560
    maximumHeight: 480
    maximumWidth: 560
    minimumWidth: 560
    minimumHeight: 480
    color:"#434343"
    flags:( Qt.Window  | Qt.FramelessWindowHint | Qt.WindowSystemMenuHint |  Qt.WindowTitleHint)
    property string activeDevice : "E:"
    property string activeFile :""
    TaskbarButton{
        id:taskBar
        overlay.iconSource: "images/image_file.png"
        overlay.accessibleDescription: "Loading"
        progress.value:  0
        progress.maximum: 100
        Component.onCompleted: {
            progress.resume()
            progress.show();
        }
    }




    MouseArea{
        anchors.fill:parent
        onClicked:{
            flashToolMain.forceActiveFocus()
        }
    }
    FlashToolMain{
        id:flashToolMain
        anchors.fill:parent
        anchors.topMargin: 40
        color:"#434343"
        onProgressChanged: taskBar.progress.value = progress
        onProgressStart:{ taskBar.progress.resume(); }
        onOperationStarted:{
            taskBar.overlay.accessibleDescription = operation;
            mainWindow.activeDevice = data[0]
            mainWindow.activeFile = data[1]
        }
        onOperationFinished:{
            mainWindow.activeFile = data
            flashToolMain.enabled = false
            mBox.visible = true
        }
    }
    TitleBar{
        id:titleBar
        anchors.top:parent.top
        anchors.left: parent.left
        anchors.right:parent.right
        height:40
        color:"#38761d"
        windowTitle.text: "ODROID Flash Tool"
        windowTitle.color:"white"
        border.width: 2
        radius:4
        windowControls.buttonColor:"#434343"
        windowControls.hoverColor: "#00ff00"
        windowControls.onMinimizeClicked: {
            mainWindow.showMinimized();
        }

        windowControls.onCloseClicked:{
            Qt.quit()
        }
    }
    Rectangle{
        visible:mBox.visible
        anchors.bottom: parent.bottom
        anchors.left:parent.left
        anchors.right:parent.right
        anchors.top:titleBar.bottom
        opacity: .9
        color:"black"
    }

    MessageBox{
        id:mBox
        visible:false
        height:parent.height * .6
        width:parent.width * .94
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        boxRibbon.color: "#38761d"
        background.color: "#434343"
        shortText.text: "Success"
        shortText.font.pixelSize: 32
        shortText.font.bold: true
        shortText.font.family: "Cambria"
        shortText.color: "white"
        messageIcon.source: "/images/check.png"
        boxRibbon.height: mBox.height * .16
        border.color: "white"
        border.width: 2
        background.border.color: "white"
        background.border.width:2
        boxRibbon.border.width: 1
        boxRibbon.border.color:"white"
        contentLoader.sourceComponent: Rectangle{
            anchors.fill: parent
            color:"transparent"
            Text{
                id:messageText
                anchors.left:parent.left
                anchors.right:parent.right
                anchors.top:parent.top
                height:28
                font.family: "Cambria"
                color:"white"
                font.pixelSize:22
                text:"Done reading image from disk " + activeDevice +"- Image saved at: "
            }
            Row{
                id:contentRow
                anchors.top:messageText.bottom
                anchors.left:parent.left
                anchors.right:parent.right
                height:38
                spacing:4
                Image{
                    anchors.verticalCenter: parent.verticalCenter
                    height: 30
                    width:height
                    source:"/images/check.png"
                }
                Rectangle{
                    width:parent.width - 42
                    height:parent.height
                    anchors.verticalCenter: parent.verticalCenter
                    border.width: 2
                    Text{
                        id: filenameText
                        anchors.fill: parent
                        anchors.margins: 4
                        text:mainWindow.activeFile
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignBottom
                        font.pixelSize: height *.6
                    }
                }
            }
            Row{
                anchors.bottom:parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.margins: 15
                width:parent.width * .8
                height: 42
                spacing:160
                Button{
                    id:finishButton
                    width:128
                    height:parent.height
                    buttonText:Text{
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text:"Finish"
                        color:"white"
                        font.pixelSize: 26
                        Component.onCompleted:
                        {
                            parent = finishButton
                        }
                    }
                    color:"#6aa84f"
                    border.color: "#b7b7b7"
                    border.width: 1
                    radius:12
                    mouseArea.hoverEnabled: true
                    mouseArea.onHoveredChanged: {
                        if(mouseArea.containsMouse)
                        {
                            finishButton.color = "#999999"
                            finishButton.border.color = "#00ff00"
                        }
                        else
                        {
                            finishButton.color = "#6aa84f"
                            finishButton.border.color = "#999999"
                        }
                    }
                    mouseArea.onClicked: {
                        flashToolMain.startOperation("Finish",mainWindow.activeFile)
                        flashToolMain.enabled = true
                        mBox.visible = false
                    }
                }
                Button{
                    id:compressButton
                    width:128
                    height:parent.height
                    buttonText:Text{
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text:"Compress"
                        color:"white"
                        font.pixelSize: 24
                        Component.onCompleted:
                        {
                            parent = compressButton
                        }
                    }
                    color:"#6aa84f"
                    border.color: "#b7b7b7"
                    border.width: 1
                    radius:12
                    mouseArea.hoverEnabled: true
                    mouseArea.onHoveredChanged: {
                        if(mouseArea.containsMouse)
                        {
                            compressButton.color = "#999999"
                            compressButton.border.color = "#00ff00"
                        }
                        else
                        {
                            compressButton.color = "#6aa84f"
                            compressButton.border.color = "#999999"
                        }
                    }
                    mouseArea.onClicked: {
                        flashToolMain.startOperation("Compress",filenameText.text)
                        flashToolMain.enabled = true
                        mBox.visible = false
                    }
                }
            }

            Component.onCompleted: {parent = mBox.contentLoader;}
        }

    }

    Component.onCompleted: {
        ODF.queryMountedDevices();
    }
}
