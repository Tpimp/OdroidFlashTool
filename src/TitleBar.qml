import QtQuick 2.6

Rectangle {
    property alias windowTitle:titleText
    property alias windowControls:windowControls
    Text{
        id:titleText
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: parent.height * .8
        anchors.top: parent.top
        anchors.bottom:parent.bottom
        width:parent.width *.28
    }
    WindowControls{
        id:windowControls
        anchors.right:parent.right
        anchors.top:parent.top
        anchors.bottom:parent.bottom
        width:150
    }
    MouseArea{
        property real mouseOldX:0
        property real mouseOldY:0
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom:parent.bottom
        anchors.right: windowControls.left
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
        preventStealing: true
    }

}
