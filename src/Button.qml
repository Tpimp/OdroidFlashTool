import QtQuick 2.6

Rectangle{
    id:buttonTop
    property alias mouseArea:mouseArea
    property Image  buttonImage:null
    property Text   buttonText:null
    MouseArea{
        id:mouseArea
        anchors.fill: parent
    }
}
