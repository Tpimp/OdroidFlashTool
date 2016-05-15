import QtQuick 2.6
import QtQuick.Window 2.2

Window {
    id:mainWindow
    visible: true
    height: 480
    width: 800
    maximumHeight: 480
    maximumWidth: 800
    minimumWidth: 800
    minimumHeight: 480
    flags:( Qt.Window  |Qt.FramelessWindowHint | Qt.WindowSystemMenuHint | Qt.WindowStaysOnTopHint | Qt.WindowTitleHint)
    TitleBar{
        id:titleBar
        anchors.top:parent.top
        anchors.left: parent.left
        anchors.right:parent.right
        height:40
        color:"#38761d"
        windowTitle.text: "ODROID Flash Tool"
        windowTitle.color:"white"
        border.width: 1
        windowControls.buttonColor:"#434343"
        windowControls.hoverColor: "#38761d"
        windowControls.onCloseClicked:{
            Qt.quit()
        }
    }
}
