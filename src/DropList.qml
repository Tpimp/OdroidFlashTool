import QtQuick 2.6

Rectangle {
    id:dropTop
    readonly property alias   currentIndex:dropList.currentIndex
    readonly property alias currentText:displayText.text
    property alias    dropHandle:dropHandle
    property alias    dropImage:dropImage
    property alias    dropText:displayText
    property alias    dropList:dropList
    property ListModel dropModel:ListModel{}
    function addItemToList(name, data)
    {
        dropModel.append({"name":name,"data":data});
        if(dropModel.count == 1)
        {
            displayText.text = name;
        }
        dropList.height = dropModel.count > 3 ? (dropTop.height *3.5):(dropTop.height * dropModel.count)
    }
    function removeItemAt(index)
    {
        if(currentIndex == index)
        {
            if(currentIndex > 0)
            {
                if(currentIndex+1 == dropModel.count)
                {
                    // if the last one
                    displayText.text = dropModel.get(currentIndex-1).name
                }
                else
                {
                    // grab the next text, but currentIndex wont change
                    displayText.text = dropModel.get(currentIndex+1).name
                }
            }
            else
            {
                if(dropModel.count > 1)
                {
                    // other items in ListElement
                    displayText.text = dropModel.get(index+1).name;
                }
                else
                {
                    // removing last one
                    displayText.text = ""
                }
            }
            dropModel.remove(index)
        }
        else
        {
            if(currentIndex > index)
            {
                dropModel.remove(index)
            }
            else
            {
                if(index < dropModel.count)
                    dropModel.remove(index)

            }
        }
        dropList.height = dropModel.count > 3 ? (dropTop.height *3.5):(dropTop.height * dropModel.count)

    }
    function clearModel()
    {
        dropModel.clear()
        currentText = ""
        currentIndex = -1;
        dropList.height = 0
    }

    Text{
        id:displayText
        anchors.fill: parent
        anchors.margins:parent.height/8
        font.pixelSize: parent.height * .74
        verticalAlignment: Text.AlignVCenter
        text:currentText
    }
    MouseArea{
        anchors.fill: parent
        onClicked: {
            dropList.visible = !dropList.visible;
            dropList.forceActiveFocus()
        }
    }
    Button{
        id:dropHandle
        anchors.right:parent.right
        anchors.top:parent.top
        anchors.bottom:parent.bottom
        anchors.margins: 4
        anchors.rightMargin: 6
        width:parent.width/4
        buttonImage:Image{
            id:dropImage
            anchors.fill: parent
            anchors.margins: 4
            fillMode: Image.PreserveAspectFit
            smooth:true
            Component.onCompleted: parent = dropHandle;
        }
        mouseArea.onClicked:{
            dropList.visible = !dropList.visible;
            dropList.forceActiveFocus()
        }
        mouseArea.hoverEnabled: true
        mouseArea.onHoveredChanged: {
            if(mouseArea.containsMouse)
            {
                dropTop.forceActiveFocus()
                dropHandle.color = "#999999"
                dropHandle.border.color = "#00ff00"
            }
            else
            {
                dropHandle.color = "#6aa84f"
                dropHandle.border.color = "#274e13"
            }
        }
    }
    ListView{
        id:dropList
        anchors.top:parent.top
        anchors.left:parent.left
        anchors.right:parent.right
        visible:false
        model:dropModel
        clip:true
        delegate:Rectangle{
            id:delegateMe
            color:dropTop.color
            border.color: dropTop.border.color
            border.width: dropTop.border.width
            width:dropTop.width
            height:dropTop.height
            radius:dropTop.radius
            Text{
                anchors.fill: parent
                anchors.margins:parent.height/8
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment:Text.AlignHCenter
                text:name
                color:displayText.color
                font.pixelSize: displayText.font.pixelSize
            }
            MouseArea{
                id:mouseArea
                anchors.fill: parent
                onClicked:{
                    dropList.currentIndex = index;
                    displayText.text = name;
                    dropList.visible = false
                }
                hoverEnabled: true
                onHoveredChanged: {
                    if(mouseArea.containsMouse)
                    {
                        delegateMe.color = dropTop.border.color
                        delegateMe.border.color = dropTop.color
                    }
                    else
                    {
                        delegateMe.color = dropTop.color
                        delegateMe.border.color = dropTop.border.color
                    }
                }
            }
        }
        onFocusChanged:  {
            if(!focus)
                dropList.visible = false
        }
    }
}
