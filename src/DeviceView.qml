import QtQuick 2.5

Rectangle {
    id:deviceTop
    color:"transparent"
    border.color: "white"
    border.width: 2
    anchors.margins: 8
    height:parent.height
    property alias  verifyAfter:verifyFlag.checked
    function getSelectedDevice()
    {
        return driveDrop.dropModel.get(driveDrop.currentIndex).itemData
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
        anchors.verticalCenter: parent.verticalCenter
        text:"Device"
        font.underline: true
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
        anchors.left:driveDrop.right
        anchors.verticalCenter: driveDrop.verticalCenter
        anchors.margins: 12
        width:28
        height:28
        source:"/images/check.png"
        checked:AppSettings.mVerifyFlash
        onCheckedChanged: {AppSettings.setVerifyFlash(verifyFlag.checked);}
    }
    Text{
        id:verifyText
        anchors.verticalCenter: verifyFlag.verticalCenter
        anchors.left: verifyFlag.right
        anchors.margins: 10
        color: "#11d011"
        font.pixelSize: 22
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text:"Use Block Verify"
    }
    DropList{
        id:driveDrop
        anchors.top:parent.top
        anchors.topMargin:8
        anchors.left: deviceText.right
        anchors.leftMargin:12
        color:"#93c47d"
        width:parent.width * .4
        height:36
        radius:6
        border.color: "white"
        border.width: 2
        dropImage.source:"images/arrow_down.png"
        dropHandle.color: "#6aa84f"
        dropHandle.border.color:"#274e13"
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


}

