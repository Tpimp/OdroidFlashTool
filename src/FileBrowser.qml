import QtQuick 2.6
import Qt.labs.folderlistmodel 2.1

Rectangle {
    id: fileBrowser
    color: "#38761d"
    signal fileSelected(string file, bool isArchive)
    signal folderSelected(string folder)
    property var lastUrl:null
    property string readTop:""
    property string currentPath:""
    property string currentFileName:""
    signal exited()

    function setCrumb(index,ctext,cdata)
    {
        cookieCrumbBar.moveToCrumb(index)
        folderModel.folder = cdata;
    }

    function openFileBrowser(browse_type,root_path,root_name,current_path)
    {
        if(browse_type >= 0)
        {
            switch(browse_type)
            {
                case 0:
                    fileNameBox.visible = false
                    break;
                case 1:
                    fileNameBox.visible = true
                    break;
                case 2:
                    folderModel.showFiles = false;
                    folderModel.resetInternalData();
                    fileNameBox.visible = false;
                    filterDrop.visible = false;
                    break;
                default:
                    break;

            }
            folderModel.rootFolder = root_path;
            if(current_path !== undefined)
            {
                //cookieCrumbBar;
                currentPath = current_path;
                var crumbs = ODF.getCrumbPath(current_path)
                cookieCrumbBar.clearCrumbs();
                for(var index =0; index < crumbs.length;)
                {
                    var current_item = crumbs[index];
                    var item_path = current_item[0];
                    var item_url = current_item[1];
                    cookieCrumbBar.appendCrumb(item_path,Qt.resolvedUrl(item_url))
                    index++;
                }
                folderModel.folder = "file:///" + Qt.resolvedUrl(current_path);
            }
            else
            {
                readTop = folderModel.rootFolder.toString();
                folderModel.folder = root_path;
                cookieCrumbBar.clearCrumbs();
                cookieCrumbBar.appendCrumb(root_name,folderModel.folder.toString());
            }
            slideEnter();
        }
        else
        {
            slideExit();
            exited();
        }

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
        anchors.bottom: buttonBack.top
        anchors.bottomMargin: 2
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
                    color:fileView.currentIndex == index ?  "lightgrey":"#434343"
                    border.color:fileView.currentIndex == index ?"black":"white"
                    border.width: 2
                    radius:6
                    height:42
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
                        color:parent.border.color
                        font.pixelSize: (width/text.length +10) < parent.height * .65 ? (width/text.length +10): parent.height * .65
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked:{
                            if(fileIsDir)
                            {
                                lastUrl = folderModel.folder;
                                folderModel.folder =  fileURL
                                cookieCrumbBar.appendCrumb(fileName,fileURL.toString())
                            }
                            else
                            {
                                fileView.currentIndex = index;
                                if(fileNameBox.visible)
                                    fileNameText.text = fileName;
                                currentFileName = fileName;

                            }
                        }
                    }
                }
            }
            FolderListModel {
                id: folderModel
                nameFilters:["*.img","*.img.xz"]
                onFolderChanged: {
                    if(folder.toString() === fileBrowser.readTop )
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
        height: 42
        color:"#6aa84f"
        DropList{
            id:filterDrop
            anchors.top: parent.top
            anchors.right:parent.right
            anchors.bottom:parent.bottom
            anchors.margins: 4
            width:parent.width * .42
            color:"#93c47d"
            radius:6
            border.color: "white"
            border.width: 2
            dropText.font.pixelSize: 14
            dropText.horizontalAlignment: Text.AlignHCenter
            onCurrentNameChanged: {
                if(currentName)
                    dropText.text = "File Filter: " + currentName
                else
                    dropText.text = "File Filter: [*.img, *.img.xz]"
            }
            dropHandle.height: 0
            dropHandle.width: 0
            dropHandle.visible: false
            dropHandle.enabled: false
            Component.onCompleted: {
                filterDrop.addItemToList("[*.img, *.img.xz]","data")
                filterDrop.addItemToList("[All Files]",null)
            }
            onCurrentDataChanged: {
                if(currentData)
                {
                    folderModel.nameFilters = ["*.img","*.img.xz"]
                }
                else
                {
                    folderModel.nameFilters = []
                }
            }
        }
        CookieCrumbBar{
            id:cookieCrumbBar
            border.color: "white"
            border.width: 1
            anchors.top:parent.top
            anchors.bottom:parent.bottom
            anchors.left:upArrow.right
            anchors.right:filterDrop.left
            color:"darkgrey"
            anchors.margins: 4
            radius:4
            crumbView.delegate: Rectangle{
                id:crumbDelegate
                border.width: 1
                radius:4
                Text{
                    id:crumbText
                    text:ctext
                    font.pixelSize: 18
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color:"white"
                    style: Text.Outline
                    styleColor:"black"
                    font.bold: true
                }
                width:ctext.length * 16
                height:cookieCrumbBar.height * .87
                color:"#6aa84f"
                MouseArea{
                    id:crumbMouse
                    anchors.fill: parent
                    onClicked:{
                        setCrumb(index,ctext,cdata);
                    }
                    hoverEnabled: true
                    onHoveredChanged: {
                        if(crumbMouse.containsMouse)
                        {
                            crumbDelegate.color = "#00ff00"
                            crumbDelegate.border.color = "black"
                            crumbText.color = "black"
                            crumbText.styleColor = "white"
                        }
                        else
                        {
                            crumbDelegate.color = "#6aa84f"
                            crumbDelegate.border.color = "white"
                            crumbText.color = "white"
                            crumbText.styleColor = "black"
                        }
                    }
                }
            }
        }

        Button{
            id:upArrow
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.margins: 4
            anchors.top:parent.top
            width:36
            radius:4
            color:"#434343"
            border.color: "#999999"
            border.width: 1
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
                    upArrow.color = "#434343"
                    upArrow.border.color = "#999999"
                }
            }
            mouseArea.onClicked:
            {
                if(cookieCrumbBar.itemCount() > 1)
                {
                    folderModel.folder = folderModel.parentFolder
                    cookieCrumbBar.popLast()
                    if(cookieCrumbBar.itemCount()  <= 1)
                        upArrow.visible = false
                }
                else
                {
                    upArrow.visible = false
                }
            }

        }
    }
    Rectangle{
        anchors.left: fileNameBox.left
        anchors.bottom: fileNameBox.top
        anchors.bottomMargin: -2
        height:28
        color:"#6aa84f"
        border.width: 2
        width:78
        radius:2
        visible:fileNameBox.visible
        Text{
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: TextInput.AlignVCenter
            anchors.fill:parent
            font.pixelSize:24
            text:"Name"
            font.family: "Cambria"
            color:"white"
        }
    }

    Rectangle{
        id:fileNameBox
        anchors.bottom: buttonBack.top
        anchors.right:checkImage.left
        height:28
        radius:8
        border.width:3
        width:parent.width/2
        clip:true
        TextInput{
            id: fileNameText
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: TextInput.AlignVCenter
            anchors.fill: parent
            anchors.margins: 4
            font.pixelSize:16
        }

    }
    Image{
        id:checkImage
        visible: fileNameText.text.length > 0
        anchors.right:parent.right
        anchors.top:fileNameBox.top
        anchors.bottom:fileNameBox.bottom
        anchors.margins: 2
        width: height
        source:"/images/check.png"
    }
    Rectangle{
        id:buttonBack
        anchors.left:parent.left
        anchors.right:parent.right
        anchors.bottom:parent.bottom
        height:parent.height * .12
        color:"darkgrey"
        radius:4
        border.width: 2
        Rectangle{
            id:buttonBar
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top:parent.top
            anchors.bottom:parent.bottom
            width:parent.width * .66
            radius:4
            color:"#6aa84f"
            border.color: "black"
            border.width:2

            Button{
                id:backButton
                anchors.left: parent.left
                width:120
                anchors.leftMargin: 4
                border.color: "white"
                border.width: 2
                anchors.bottom:parent.bottom
                anchors.top:parent.top
                anchors.margins: 4
                radius: 6
                color: "#434343"
                buttonImage:Image{
                    id:backImage
                    width: 36
                    height:36
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    source:"/images/arrow_down.png"
                    rotation:90
                    fillMode: Image.PreserveAspectFit
                    smooth:true
                    Component.onCompleted: parent = backButton;
                }
                buttonText:Text{
                    anchors.top:parent.top
                    anchors.bottom:parent.bottom
                    anchors.right:parent.right
                    anchors.left: backImage.right
                    anchors.leftMargin: -4
                    text:  "Back"
                    color: parent.border.color
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: parent.height * .48
                    Component.onCompleted: parent = backButton
                }
                mouseArea.hoverEnabled: true
                mouseArea.onHoveredChanged: {
                    if(mouseArea.containsMouse)
                    {
                        backButton.color = "lightgrey"
                        backButton.border.color = "black"
                    }
                    else
                    {
                        backButton.color = "#434343"
                        backButton.border.color = "white"
                    }
                }
                mouseArea.onClicked: {
                    fileBrowser.slideExit();
                }

            }
            Button{
                id:imageFolder
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                height:backButton.height
                width:height
                anchors.leftMargin: 2
                border.color: "white"
                border.width: 2
                anchors.margins: 2
                radius: 4
                color: "#434343"
                mouseArea.hoverEnabled:true
                mouseArea.onHoveredChanged: {
                    if(mouseArea.containsMouse)
                        border.color = "#00ff00";
                    else
                        border.color = "white"
                }
                buttonImage:Image{
                    id:imageFolderIcon
                    height:parent.height * .86
                    width:height
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.horizontalCenterOffset: -1
                    anchors.verticalCenter: parent.verticalCenter
                    source:"/images/folder.png"
                    fillMode: Image.PreserveAspectFit
                    smooth:true
                    Component.onCompleted: parent = imageFolder;
                }
                mouseArea.onClicked: {
                    var crumbs;
                    var path;
                    if(currentPath !== "")
                    {
                        path = "file:///" + Qt.resolvedUrl(currentPath)
                        crumbs = ODF.getCrumbPath(currentPath)
                    }
                    else
                    {
                        path = ODF.temporaryUrl();
                        crumbs = ODF.temporaryCrumbs()
                    }
                    cookieCrumbBar.clearCrumbs();
                    for(var index =0; index < crumbs.length;)
                    {
                        var current_item = crumbs[index];
                        var item_path = current_item[0];
                        var item_url = current_item[1];
                        cookieCrumbBar.appendCrumb(item_path,Qt.resolvedUrl(item_url))
                        index++;
                    }

                    folderModel.folder = path;
                }
            }
            Button{
                id:chooseButton
                anchors.right: parent.right
                width:120
                anchors.rightMargin: 4
                border.color: "white"
                border.width: 2
                anchors.bottom:parent.bottom
                anchors.top:parent.top
                anchors.margins: 4
                radius: 6
                color: "#434343"
                buttonText:Text{

                    anchors.fill:parent
                    anchors.leftMargin: 4
                    text:  "Choose"
                    color: parent.border.color
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: parent.height * .48
                    Component.onCompleted: parent = chooseButton
                }
                mouseArea.hoverEnabled: true
                mouseArea.onHoveredChanged: {
                    if(mouseArea.containsMouse)
                    {
                        chooseButton.color = "lightgrey"
                        chooseButton.border.color = "black"
                    }
                    else
                    {
                        chooseButton.color = "#434343"
                        chooseButton.border.color = "white"
                    }
                }
                mouseArea.onClicked: {
                    if(fileView.currentIndex >= 0)
                    {
                        var fileName;
                        var file_str;
                        if(fileNameBox.visible)
                        {
                            fileName = fileNameText.text;
                            file_str = fileName.substring(fileName.length-3,fileName.length)
                            fileBrowser.fileSelected(cookieCrumbBar.crumbPath + '/' + fileName, file_str === ".xz")
                        }
                        else
                        {
                            fileName = currentFileName;
                            file_str = fileName.substring(fileName.length-3,fileName.length)
                            fileBrowser.fileSelected(cookieCrumbBar.crumbPath + '/' + fileName, file_str === ".xz")
                        }
                    }
                    else
                    {
                        var path = folderModel.folder.toString();
                        fileBrowser.folderSelected(path.replace("file:///",""));
                    }
                    fileBrowser.slideExit()
                }

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
    Component.onCompleted: {
    }
}
