import QtQuick 2.6

Rectangle {
    ImageFileView{
        id:imageView
        height:parent.height *.42
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
    }
    DeviceView{
        height:parent.height *.3
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: imageView.bottom
    }


}
