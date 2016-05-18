import QtQuick 2.6

Rectangle {
    id:titleTop
    property alias windowTitle:titleText
    property alias windowControls:windowControls
    property bool  settingsOpen:false
    Text{
        id:titleText
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: -parent.width/8
        font.pixelSize: 36
        anchors.top: parent.top
        width:parent.width *.28
        height:40
    }
    onSettingsOpenChanged:
    {
        if(settingsOpen)
        {
            openSettings()
        }
        else
        {
            closeSettings();
        }
    }

    function closeSettings()
    {
        closeAnimation.start();
    }

    function openSettings()
    {
        openAnimation.start();
    }

    Button{
        id:settingsButton
        anchors.left:parent.left
        anchors.top:parent.top
        height:34
        width:height
        anchors.margins: 3
        border.width:2
        radius:4
        color:"#93c47d"
        buttonImage:Image{
            source:"images/settings.png"
            anchors.fill: parent
            fillMode:Image.PreserveAspectFit
            smooth:true
            anchors.margins:4
            Component.onCompleted: parent = settingsButton;
        }
        mouseArea.onClicked: settingsOpen = !settingsOpen;

    }


    MouseArea{
        property real mouseOldX:0
        property real mouseOldY:0
        anchors.left: settingsButton.right
        anchors.top: parent.top
        anchors.bottom:parent.bottom
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

    WindowControls{
        id:windowControls
        anchors.right:parent.right
        anchors.top:parent.top
        width:150
        height:38
    }

    NumberAnimation{
        id:openAnimation
        target:titleTop
        property:"height"
        duration:300
        alwaysRunToEnd: true
        from:40
        to:160
        running: false
    }
    NumberAnimation{
        id:closeAnimation
        target:titleTop
        property:"height"
        duration:150
        alwaysRunToEnd: true
        from:160
        to:40
        running:false
    }

}
