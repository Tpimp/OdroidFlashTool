import QtQuick 2.6
import Qt.labs.folderlistmodel 2.1

Rectangle {
    id: fileBrowser
    color: "#999999"
    signal fileSelected(string file, bool isArchive)
    property var lastUrl:null
    property string readTop:""
    signal exited()
    function openFileBrowser(read,path)
    {
        if(read)
        {
            fileNameBox.visible = false
            titleText.text = "Locate your image [*.img, *.img.xz]"
        }
        else
        {
            fileNameBox.visible = true
            titleText.text = "Directory to store the image"
        }
        folderModel.rootFolder = path;
        readTop = folderModel.rootFolder.toString();
        folderModel.folder = path;
        slideEnter();

    }
    function slideExit()
    {
        fileBrowser.x = width +1
    }
    function slideEnter()
    {
        fileBrowser.visible = true;
        fileBrowser.x = 0

    }

    height:400
    width:400


    Rectangle{
        color:"transparent"
        anchors.left:parent.left
        anchors.right: parent.right
        anchors.bottom: buttonBar.top
        anchors.top:titleRect.bottom
        anchors.leftMargin: parent.width * .01
        anchors.rightMargin: parent.width * .01
        ListView{
            anchors.fill: parent
            id:fileView
            spacing:2
            Component{
                id:fileDelegate

                Rectangle{
                    color:"#434343"
                    border.color:"white"
                    border.width: 2
                    radius:8
                    height:62
                    width:parent.width
                    property alias imageSource:dirImage.source
                    Image{
                        id:dirImage
                        anchors.left:parent.left
                        anchors.bottom: parent.bottom
                        anchors.top:parent.top
                        anchors.margins: 4
                        width: height * 1.25
                        smooth:true
                        fillMode: Image.PreserveAspectFit
                        Component.onCompleted:{
                            if(fileIsDir)
                            {
                                source = "/images/folder_sm.png"
                            }
                            else
                            {
                                var file_str = fileName.substring(fileName.length-3,fileName.length)
                                if(file_str == ".xz")
                                {
                                    source = "/images/archive_file.png"
                                }
                                if(file_str == "img")
                                {
                                    source = "/images/image_file.png"
                                }
                            }
                        }
                    }
                    Text{
                        id:fileText
                        anchors.left:dirImage.right
                        anchors.leftMargin:4
                        anchors.verticalCenter: parent.verticalCenter
                        verticalAlignment: Text.AlignVCenter
                        height:parent.height
                        anchors.rightMargin:4
                        anchors.right: parent.right
                        text:fileName
                        color:"white"
                        font.pixelSize: (width/text.length +10) < parent.height * .65 ? (width/text.length +10): parent.height * .65
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked:{
                            if(fileName === "." || fileName ==="..")
                            {
                                lastUrl = folderModel.folder;
                                folderModel.folder =  fileURL
                                currDir.text = fileURL.toString().substring(8)
                            }
                            if(fileIsDir)
                            {
                                lastUrl = folderModel.folder;
                                folderModel.folder =  fileURL
                                currDir.text = fileURL.toString().substring(8)
                            }
                            else
                            {
                                fileView.currentIndex = index;
                                var file_str = fileName.substring(fileName.length-3,fileName.length)
                                fileBrowser.fileSelected(currDir.text + '/' + fileName, file_str === ".xz")
                            }
                        }
                    }
                }
            }
            FolderListModel {
                id: folderModel
                nameFilters:["*.img", "*.img.xz"]
                onFolderChanged: {
                    var folder_str = folder.toString()
                    if(folder_str === readTop )
                        upArrow.visible = false;
                    else
                        upArrow.visible = true;
                }
                Component.onCompleted: {
                    rootFolder = folder
                }
            }
            delegate: fileDelegate
            model:folderModel
        }
    }
    Rectangle{
        id:titleRect
        anchors.left:parent.left
        anchors.right:parent.right
        anchors.top:parent.top
        border.width: 2
        radius:2
        height: 36
        color:"#93c47d"
        Text{
            id:titleText
            anchors.fill:parent
            text:"Locate your image [*.img, *.img.xz]"
            font.pixelSize: parent.height * .65
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
        Button{
            id:upArrow
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.margins: 2
            anchors.leftMargin:8
            anchors.top:parent.top
            width:parent.width/10
            radius:4
            color:"#6aa84f"
            border.color: "#999999"
            border.width: 2
            buttonImage:Image{
                anchors.fill: parent
                anchors.margins:4
                source:"/images/arrow_down.png"
                rotation:180
                fillMode: Image.PreserveAspectFit
                smooth:true
                Component.onCompleted: parent = upArrow;
            }
            mouseArea.hoverEnabled: true
            mouseArea.onHoveredChanged: {
                if(mouseArea.containsMouse)
                {
                    upArrow.color = "#999999"
                    upArrow.border.color = "#00ff00"
                }
                else
                {
                    upArrow.color = "#6aa84f"
                    upArrow.border.color = "#999999"
                }
            }
            mouseArea.onClicked:
            {
                if(folderModel.folder.toString() !== folderModel.rootFolder.toString())
                {
                    currDir.text = folderModel.parentFolder.toString().substring(8)
                    folderModel.folder = folderModel.parentFolder
                }
            }

        }
    }
    Rectangle{
        anchors.right: parent.right
        anchors.bottom: fileNameBox.top
        height:22
        color:"#6aa84f"
        border.width: 2
        width:60
        radius:2
        visible:fileNameBox.visible
        Text{
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: TextInput.AlignVCenter
            anchors.fill:parent
            anchors.margins: 4
            font.pixelSize:19
            text:"Name"
        }
    }

    Rectangle{
        id:fileNameBox
        anchors.bottom: buttonBar.top
        anchors.right:parent.right
        height:28
        radius:8
        border.width:3
        width:parent.width/2
        clip:true
        TextInput{
            id: fileNameText
            horizontalAlignment: Text.AlignRight
            verticalAlignment: TextInput.AlignVCenter
            anchors.fill:parent
            anchors.margins: 4
            font.pixelSize:18
        }
        Image{
            id:checkImage
            visible: fileNameText.text.length > 0
            anchors.left:parent.left
            anchors.top:parent.top
            anchors.bottom:parent.bottom
            anchors.margins: 2
            width: height
            source:"/images/check.png"
            MouseArea{
                anchors.fill: parent
                onClicked:{
                    // choose location and name
                    var file_str = fileNameText.text.substring(fileNameText.text.length-3,fileNameText.text.length)
                    fileBrowser.fileSelected(currDir.text + '/' + fileNameText.text, file_str === ".xz")
                }
            }
        }
    }
    Rectangle{
        id:buttonBar
        anchors.left:parent.left
        anchors.right:parent.right
        anchors.bottom:parent.bottom
        height:48
        radius:4
        border.width: 2
        color:"#38761d"
        border.color: "white"

        Text{
            id:currDir
            anchors.right: parent.right
            anchors.top:parent.top
            anchors.bottom:parent.bottom
            width:parent.width * .51
            anchors.margins: 4
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
            color:"white"
            font.pixelSize: (width/text.length +8) < parent.height * .35 ? (width/text.length +8): parent.height * .35
        }
        Button{
            id:backButton
            anchors.left: parent.left
            width:parent.width * .48
            anchors.leftMargin: 2
            border.color: "#434343"
            border.width: 2
            anchors.bottom:parent.bottom
            anchors.top:parent.top
            anchors.margins: 2
            radius: 4
            color: "#434343"
            buttonText:Text{
                anchors.fill: parent
                text:  "Back"
                color:"white"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: parent.height * .55
                Component.onCompleted: parent = backButton
            }
            mouseArea.onClicked: {
                fileBrowser.slideExit();
            }

        }
    }
    Behavior on x{
        NumberAnimation{
            duration:650
            alwaysRunToEnd: true
        }
    }
    onXChanged:
    {
        if(fileBrowser.x == fileBrowser.width+1)
        {
            fileBrowser.visible = false;
            fileBrowser.exited();
        }
    }
}
