import QtQuick 2.6
import QtQuick.Window 2.2

Window {
    id:mainWindow
    visible: true
    height: 480
    width: 560
    maximumHeight: 480
    maximumWidth: 560
    minimumWidth: 560
    minimumHeight: 480
    color:"#434343"
    flags:( Qt.Window  | Qt.FramelessWindowHint | Qt.WindowSystemMenuHint |  Qt.WindowTitleHint)

    MouseArea{
        anchors.fill:parent
        onClicked:{
            flashToolMain.forceActiveFocus()
        }
    }
    FlashToolMain{
        id:flashToolMain
        anchors.fill:parent
        anchors.topMargin: 40
        color:"#434343"
    }
    TitleBar{
        id:titleBar
        anchors.top:parent.top
        anchors.left: parent.left
        anchors.right:parent.right
        height:40
        color:"#38761d"
        windowTitle.text: "ODROID Flash Tool"
        windowTitle.color:"white"
        border.width: 2
        radius:4
        windowControls.buttonColor:"#434343"
        windowControls.hoverColor: "#00ff00"
        windowControls.onMinimizeClicked: {
            mainWindow.showMinimized();
        }

        windowControls.onCloseClicked:{
            Qt.quit()
        }
    }
    Component.onCompleted: {
        ODF.queryMountedDevices();
    }
}
