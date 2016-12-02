import QtQuick 2.5

Rectangle {
    id:deviceTop
    color:"transparent"
    border.color: "white"
    border.width: 2
    anchors.margins: 8
    height:parent.height
    property real deviceId: 0
    signal openBootSettings(real deviceId)
    function getSelectedDevice()
    {
        return driveDrop.dropModel.get(driveDrop.currentIndex).itemData
    }
    function selectCurrentTarget(target)
    {
        switch(target)
        {
            case 0: c2Flag.clearCheck();xUFlag.clearCheck(); u3Flag.clearCheck();deviceId = 0; break;
            case 1: c1Flag.clearCheck();xUFlag.clearCheck(); u3Flag.clearCheck();deviceId = 1; break;
            case 2: c1Flag.clearCheck();c2Flag.clearCheck(); u3Flag.clearCheck();deviceId = 2; break;
            case 3: c1Flag.clearCheck();c2Flag.clearCheck(); xUFlag.clearCheck();deviceId = 3; break;
            default: break;
        }
    }



    Connections{
        target:ODF
        onFoundMountedDevice:
        {
            if(device === root_path)
            {
                driveDrop.addItemToList("Drive:"+" (" +device+")",root_path)
            }
            else
                driveDrop.addItemToList(device +" (" +root_path+")" ,root_path)
        }

    }
    Connections{
        target:AppSettings
        onShowNotReadyDisksChanged:
        {
            driveDrop.clearModel();
        }
    }

    Text{
        id:deviceText
        anchors.left:parent.left
        anchors.leftMargin: 8
        anchors.top: parent.top
        anchors.topMargin:8
        text:"Media Device"
        font.underline: true
        font.pixelSize: 25
        font.bold: true
        color:"white"
    }

    DropList{
        id:driveDrop
        anchors.top:deviceText.bottom
        anchors.topMargin:8
        anchors.left: parent.left
        anchors.leftMargin:16
        color:"#93c47d"
        width:parent.width * .46
        height:36
        radius:6
        border.color: "white"
        border.width: 2
        dropImage.source:"images/arrow_down.png"
        dropHandle.color: "#6aa84f"
        dropHandle.border.color:"#274e13"
        dropListFrame.border.color:"orange"
        dropHandle.border.width: 2
        dropHandle.width: driveDrop.width * .12
        dropHandle.height: driveDrop.height * .6
        dropText.font.pixelSize: 16
        onCurrentNameChanged:
        {
            dropText.text = currentName
        }
        dropHandle.mouseArea.onClicked: {
            driveDrop.clearModel()
            ODF.queryMountedDevices();
        }
        mouseArea.onClicked: {
            driveDrop.clearModel()
            ODF.queryMountedDevices();
        }
    }

    Text{
        id:targetText
        anchors.right:parent.right
        anchors.rightMargin:6
        color: "#ffffff"
        font.pixelSize: 20
        font.bold: true
        horizontalAlignment: Text.AlignRight
        verticalAlignment: Text.AlignVCenter
        anchors.top: parent.top
        anchors.topMargin: 6
        font.underline: true
        text:"Target Odroid"
    }
    RadioButton{
        id:c1Flag
        color:"transparent"
        radius:4
        border.color: "#00ff00"
        border.width: 2
        anchors.right:c1Text.left
        anchors.top:targetText.bottom
        anchors.margins: 4
        anchors.topMargin: 6
        width:18
        height:18
        source:"/images/check.png"
        mouseArea.hoverEnabled:true
        onCheckedChanged: {deviceTop.selectCurrentTarget(0); c1Flag.checked = true;}
        mouseArea.onHoveredChanged: {
            if(c1Flag.mouseArea.containsMouse)
            {
                c1Flag.border.color = "white"
            }
            else
            {
                c1Flag.border.color= "#00ff00"
            }
        }
    }
    Text{
        id:c1Text
        anchors.verticalCenter: c1Flag.verticalCenter
        anchors.right: c2Flag.left
        anchors.margins: 4
        anchors.rightMargin: 8
        color: "#ffffff"
        font.pixelSize: 14
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text:"C1"
    }
    RadioButton{
        id:c2Flag
        color:"transparent"
        radius:4
        border.color: "#00ff00"
        border.width: 2
        anchors.right:c2Text.left
        anchors.margins: 4
        anchors.verticalCenter: c1Flag.verticalCenter
        width: 18
        height: 18
        source:"/images/check.png"
        mouseArea.hoverEnabled:true
        onCheckedChanged: {deviceTop.selectCurrentTarget(1); c2Flag.checked = true;}
        mouseArea.onHoveredChanged: {
            if(c2Flag.mouseArea.containsMouse)
            {
                c2Flag.border.color = "white"
            }
            else
            {
                c2Flag.border.color= "#00ff00"
            }
        }
    }
    Text{
        id:c2Text
        anchors.verticalCenter: c1Flag.verticalCenter
        anchors.right: xUFlag.left
        anchors.margins: 4
        anchors.rightMargin: 8
        color: "#ffffff"
        font.pixelSize: 14
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text:"C2"
    }
    RadioButton{
        id:xUFlag
        color:"transparent"
        radius:4
        border.color: "#00ff00"
        border.width: 2
        anchors.right:xUText.left
        anchors.verticalCenter: c1Flag.verticalCenter
        anchors.margins: 4
        width: 18
        height: 18
        source:"/images/check.png"
        mouseArea.hoverEnabled:true
        onCheckedChanged: {deviceTop.selectCurrentTarget(2); xUFlag.checked = true;}
        mouseArea.onHoveredChanged: {
            if(xUFlag.mouseArea.containsMouse)
            {
                xUFlag.border.color = "white"
            }
            else
            {
                xUFlag.border.color= "#00ff00"
            }
        }
    }
    Text{
        id:xUText
        anchors.verticalCenter: c1Flag.verticalCenter
        anchors.right: u3Flag.left
        anchors.margins: 4
        anchors.rightMargin: 8
        color: "#ffffff"
        font.pixelSize: 14
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text:"Xu4"
    }
    RadioButton{
        id:u3Flag
        color:"transparent"
        radius:4
        border.color: "#00ff00"
        border.width: 2
        anchors.right: u3Text.left
        anchors.verticalCenter: c1Flag.verticalCenter
        anchors.margins: 4
        width: 18
        height: 18
        source:"/images/check.png"
        mouseArea.hoverEnabled:true
        onCheckedChanged: {deviceTop.selectCurrentTarget(3); u3Flag.checked = true;}
        mouseArea.onHoveredChanged: {
            if(u3Flag.mouseArea.containsMouse)
            {
                u3Flag.border.color = "white"
            }
            else
            {
                u3Flag.border.color= "#00ff00"
            }
        }
    }
    Text{
        id:u3Text
        anchors.verticalCenter: c1Flag.verticalCenter
        anchors.right: parent.right
        anchors.margins: 4
        anchors.rightMargin: 10
        color: "#ffffff"
        font.pixelSize: 14
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text:"U3"
    }
    BootView{
        id:bootView
        height:parent.height *.50
        width:parent.width *.4
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 4
    }
    Component.onCompleted: c1Flag.checked = true;
}

