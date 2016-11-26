import QtQuick 2.6
import QtMultimedia 5.5
Rectangle {
    id:titleTop
    property alias windowTitle:titleText
    property alias windowControls:windowControls
    property bool  settingsOpen:false
    Text{
        id:titleText
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: -parent.width/8
        font.pixelSize: 32
        anchors.top: parent.top
        width:parent.width *.28
        height:40
        font.family: "Caladea"
        font.italic: true
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

    SoundEffect {
        id: openSound
        source: "qrc:/sounds/SettingsOpen.wav"
        volume: .35
    }
    SoundEffect {
        id: closeSound
        source: "qrc:/sounds/SettingsClose.wav"
        volume:.35
    }
    function closeSettings()
    {
        appSettings.closeOut()
        settingsButton.enabled = false
        appSettings.enabled = false
        appSettings.opacity = 0;
        closeAnimation.start();
        closeSound.play();
        AppSettings.saveSettings();
    }

    function openSettings()
    {
        settingsButton.enabled = false
        appSettings.enabled = true
        appSettings.visible = true
        appSettings.opacity = 1.0
        openSound.play();
        openAnimation.start();

    }




    MouseArea{
        property real mouseOldX:0
        property real mouseOldY:0
        anchors.left: parent.left
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
            source:"release/OdroidFlashTool.ico"
            anchors.fill: parent
            fillMode:Image.PreserveAspectFit
            smooth:true
            anchors.margins:4
            Component.onCompleted: parent = settingsButton;
        }
        mouseArea.onClicked: settingsOpen = !settingsOpen;
        mouseArea.hoverEnabled: true
        mouseArea.onEntered:
        {
            color = "#00ff00"
        }
        mouseArea.onExited:
        {
            color = "#93c47d";
        }
    }
    NumberAnimation{
        id:openAnimation
        target:titleTop
        property:"height"
        duration:400
        alwaysRunToEnd: true
        from:40
        to:480
        running: false
        onStopped:{
            settingsButton.enabled = true
        }
    }
    NumberAnimation{
        id:closeAnimation
        target:titleTop
        property:"height"
        duration:200
        alwaysRunToEnd: true
        from:480
        to:40
        running:false
        onStopped: {
            appSettings.visible = false
            settingsButton.enabled = true
        }
    }
    ApplicationSettings{
        id:appSettings
        anchors.top: settingsButton.bottom
        anchors.margins:2
        anchors.bottom: parent.bottom
        anchors.left:parent.left
        anchors.right:parent.right
        visible:false
        opacity:0
        enabled:false
        Behavior on opacity{
            NumberAnimation{duration:550;}
        }
    }
}
