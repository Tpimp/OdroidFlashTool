import QtQuick 2.6

Rectangle {
    id:dialogTop
    property alias background:internalRect
    property alias shortText:shortText
    property alias boxRibbon:ribbon
    property real  textPixelSize:24
    property color textColor:"white"
    property alias messageIcon:messageIcon
    property alias contentArea:contentArea
    property alias contentLoader:contentLoader
    signal okButtonClicked()
    radius:12

    Rectangle{
        id:internalRect
        anchors.fill: parent
        radius:parent.radius
    }


    Rectangle{
        id:ribbon
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        radius:8
        anchors.topMargin: 2
        anchors.leftMargin:1
        anchors.rightMargin:1
        border.width: 2
        height:40
    }
    Text{
        id:shortText
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top:ribbon.bottom
        anchors.margins: 8
        font.pixelSize: textPixelSize
        //textColor: dialogTop.textColor
    }
    Rectangle{
        width:parent.width * .18
        height:width
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top:parent.top
        anchors.topMargin: -height/2
        border.color: "white"
        border.width: 6
        color:"#14db02"
        radius:32
        Image{
            id:messageIcon
            anchors.fill: parent
            anchors.margins: parent.height * .16
            source:""
        }
    }
    MouseArea{
        property real mouseOldX:0
        property real mouseOldY:0
        anchors.fill: parent
        width:parent.width
        onPressed: {
            mouseOldX = mouseX;
            mouseOldY = mouseY;
        }
        onMouseXChanged: {
            mainWindow.x += (mouseX - mouseOldX);
        }

        onMouseYChanged: {
            mainWindow.y += (mouseY - mouseOldY);
        }
    }
    Rectangle{
        id:contentArea
        color:"transparent"
        anchors.top: shortText.bottom
        anchors.bottom:parent.bottom
        anchors.left:parent.left
        anchors.right:parent.right
        anchors.margins: 8
        anchors.topMargin: 0
        Loader{
            id:contentLoader
            anchors.fill: parent
        }
    }
}
