import QtQuick 2.6


Rectangle{
    id:appSettingsTop
    border.width: 2
    border.color: "white"
    color:"#434343"
    function closeOut()
    {
        fileBrowserLoader.active = false
    }

    Rectangle{
        id:leftBar
        height:2
        width:parent.width *.4
        color:"transparent"
        border.color: "white"
        border.width: 1
        anchors.left:parent.left
        anchors.top: parent.top
        anchors.margins: 4
        anchors.topMargin: 16
    }
    Rectangle{
        id:rightBar
        height:2
        width:parent.width *.4
        color:"transparent"
        border.color: "white"
        border.width: 1
        anchors.right:parent.right
        anchors.margins: 4
        anchors.top: parent.top
        anchors.topMargin:16
    }

    Text{
        id:settingsTitle
        anchors.left:leftBar.right
        anchors.right:rightBar.left
        anchors.margins: 2
        font.pixelSize: 22
        text:"Settings"
        font.family: "Magneto"
        anchors.top: parent.top
        color:"white"
        height:36
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignTop
    }
    Rectangle{
        border.color: "white"
        color:"#434343"
        border.width: 1
        height:42
        anchors.horizontalCenter: parent.horizontalCenter
        width:parent.width*.9
        anchors.top: rightBar.bottom
        anchors.topMargin: 12
        id:dadRect
        RadioButton{
            id:deleteAfterDecompress
            checked: AppSettings.mDeleteAfterDecompress
            width:height
            color:"transparent"
            radius:4
            border.color: "#00ff00"
            border.width: 2
            source:"/images/check.png"
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: 4
            anchors.bottom:parent.bottom
            mouseArea.onClicked:  AppSettings.setDeleteAfterDecompress(deleteAfterDecompress.checked)
        }
        Text{
            anchors.left: deleteAfterDecompress.right
            anchors.right:parent.right
            anchors.top: parent.top
            anchors.bottom:parent.bottom
            text:"Delete the Image after Decompressed and Flashed"
            font.pixelSize: 18
            anchors.margins: 20
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color:"white"
        }
    }
    Rectangle{
        border.color: "white"
        color:"#434343"
        border.width: 1
        height:42
        anchors.horizontalCenter: parent.horizontalCenter
        width:parent.width*.9
        anchors.top: dadRect.bottom
        anchors.margins: 8
        id: dacRect
        RadioButton{
            id:deleteAfterCompress
            checked: AppSettings.mDeleteAfterCompress
            width:height
            color:"transparent"
            radius:4
            border.color: "#00ff00"
            border.width: 2
            source:"/images/check.png"
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: 4
            anchors.bottom:parent.bottom
            mouseArea.onClicked:  AppSettings.setDeleteAfterCompress(deleteAfterCompress.checked)
        }
        Text{
            anchors.left: deleteAfterCompress.right
            anchors.right:parent.right
            anchors.top: parent.top
            anchors.bottom:parent.bottom
            text:"Delete the Image when Compression is Complete"
            font.pixelSize: 18
            anchors.margins: 20
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color:"white"
        }
    }
    Rectangle{
        border.color: "white"
        color:"#434343"
        border.width: 1
        height:42
        anchors.horizontalCenter: parent.horizontalCenter
        width:parent.width*.9
        anchors.top: dacRect.bottom
        anchors.margins: 8
        id:snrRect
        RadioButton{
            id:showNotReadyDisks
            checked: AppSettings.mShowNotReadyDisks
            width:height
            color:"transparent"
            radius:4
            border.color: "#00ff00"
            border.width: 2
            source:"/images/check.png"
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: 4
            anchors.bottom:parent.bottom
            mouseArea.onClicked:  AppSettings.setShowNotReadyDisks(showNotReadyDisks.checked)
        }
        Text{
            anchors.left: showNotReadyDisks.right
            anchors.right:parent.right
            anchors.top: parent.top
            anchors.bottom:parent.bottom
            text:"Show Empty Disk Drives (No Media)"
            font.pixelSize: 18
            anchors.margins: 20
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color:"white"
        }
    }
    Rectangle{
        border.color: "white"
        color:"#434343"
        border.width: 1
        height:42
        anchors.horizontalCenter: parent.horizontalCenter
        width:parent.width*.9
        anchors.top: snrRect.bottom
        anchors.margins: 8
        id:showDecompressDialog
        RadioButton{
            id:showDecompress
            checked: AppSettings.mShowDecompressDialog
            width:height
            color:"transparent"
            radius:4
            border.color: "#00ff00"
            border.width: 2
            source:"/images/check.png"
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: 4
            anchors.bottom:parent.bottom
            mouseArea.onClicked:  AppSettings.setShowDecompressDialog(showDecompress.checked)
        }
        Text{
            anchors.left: showDecompress.right
            anchors.right:parent.right
            anchors.top: parent.top
            anchors.bottom:parent.bottom
            text:"Show Dialog before Starting to Write"
            font.pixelSize: 18
            anchors.margins: 20
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color:"white"
        }
    }
    Rectangle{
        border.color: "white"
        color:"#434343"
        border.width: 1
        height:42
        anchors.horizontalCenter: parent.horizontalCenter
        width:parent.width*.9
        anchors.top: showDecompressDialog.bottom
        anchors.margins: 8
        id:showCompressDialog
        RadioButton{
            id:showCompress
            checked: AppSettings.mShowCompressDialog
            width:height
            color:"transparent"
            radius:4
            border.color: "#00ff00"
            border.width: 2
            source:"/images/check.png"
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: 4
            anchors.bottom:parent.bottom
            mouseArea.onClicked:  AppSettings.setShowCompressDialog(showCompress.checked)
        }
        Text{
            anchors.left: showCompress.right
            anchors.right:parent.right
            anchors.top: parent.top
            anchors.bottom:parent.bottom
            text:"Show Dialog before Starting to Compress"
            font.pixelSize: 18
            anchors.margins: 20
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color:"white"
        }
    }
    Rectangle{
        border.color: "white"
        color:"#434343"
        border.width: 1
        height:42
        anchors.horizontalCenter: parent.horizontalCenter
        width:parent.width*.9
        anchors.top: showCompressDialog.bottom
        anchors.margins: 8
        id:usePartitionsOnly
        RadioButton{
            id:partitionsOnly
            checked: AppSettings.mPartitionsOnly
            width:height
            color:"transparent"
            radius:4
            border.color: "#00ff00"
            border.width: 2
            source:"/images/check.png"
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: 4
            anchors.bottom:parent.bottom
            mouseArea.onClicked:  AppSettings.setUsePartitionsOnly(partitionsOnly.checked)
        }
        Text{
            anchors.left: partitionsOnly.right
            anchors.right:parent.right
            anchors.top: parent.top
            anchors.bottom:parent.bottom
            text:"Copy/Write Valid Disk Partitions(no Free space)"
            font.pixelSize: 18
            anchors.margins: 20
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color:"white"
        }
    }
    Rectangle{
        border.color: "white"
        color:"#434343"
        border.width: 1
        height:80
        anchors.horizontalCenter: parent.horizontalCenter
        width:parent.width*.9
        anchors.top: usePartitionsOnly.bottom
        anchors.margins: 8
        id:workDirectoryRect
        Text{
            anchors.left: parent.left
            anchors.right:parent.right
            anchors.top: parent.top
            height:24
            text:"Directory to Work with and Store Images"
            font.pixelSize: 24
            anchors.margins: 4
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color:"white"
        }

        Rectangle{
            id:filenameContainer
            radius:8
            anchors.right:fileOpenButton.left
            height: 40
            border.color: "#6aa84f"
            border.width: 2
            anchors.bottom:parent.bottom
            anchors.bottomMargin: 4
            anchors.rightMargin: 10
            anchors.left:parent.left
            anchors.leftMargin: 8
            clip:true
            TextInput{
                id: workDirectoryPath
                anchors.fill: parent
                anchors.leftMargin: 6
                anchors.rightMargin: 6
                horizontalAlignment: TextInput.AlignLeft
                verticalAlignment: TextInput.AlignVCenter
                enabled:false
                font.pixelSize: (width/text.length+ 6) < parent.height *.6 ? (width/text.length+ 6):parent.height *.6
            }
        }
        Button{
            id:fileOpenButton
            anchors.bottom: filenameContainer.bottom
            anchors.right:parent.right
            height:42
            width:height
            radius:4
            color:"#6aa84f"
            border.color: "#999999"
            border.width: 2
            anchors.rightMargin: 8
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
            mouseArea.onClicked: fileBrowserLoader.active = true
        }
    }
    Loader{
        id:fileBrowserLoader
        anchors.fill: parent
        sourceComponent:FileBrowser{
            id:fileBrowser
            visible:false
            width:appSettingsTop.width
            height:appSettingsTop.height
            x:appSettingsTop.width
            y:0
            onFolderSelected: {
                workDirectoryPath.text = folder
                AppSettings.setWorkDirectory(folder);
            }
            Component.onCompleted:
            {
                fileBrowser.openFileBrowser(2,ODF.getSystemRootUrl(),ODF.getSystemRootName(), AppSettings.mWorkDirectory)
            }
            onExited:
            {
                fileBrowserLoader.active = false
            }
        }
        active:false
    }
    Component.onCompleted: {
        workDirectoryPath.text = AppSettings.mWorkDirectory;
    }
}
