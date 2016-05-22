import QtQuick 2.5

Rectangle{
    id:buttonTop
    property alias mouseArea:mouseArea
    property Image  checkImage:null
    property bool checked:false
    property string source:""
    onCheckedChanged: {
        if(checked)
        {
            createCheck()
        }
        else
        {
            clearCheck()
        }
    }

    function createCheck()
    {
        if(checkImage != null)
            return;

            buttonTop.checkImage = Qt.createQmlObject('import QtQuick 2.5; Image {id:image;}', buttonTop);
            if(checkImage != null)
            {
                checkImage.source = source
                checkImage.anchors.margins = 1;
                checkImage.onSourceChanged.connect(function(){
                    if(checkImage.source == "")
                        checkImage.destroy();
                })
                checkImage.anchors.fill = buttonTop
            }

    }
    function clearCheck()
    {
        if(checkImage != null)
        {
            buttonTop.checkImage.visible = false;
            buttonTop.checkImage.source = ""
            buttonTop.checkImage = null;
        }
    }
    MouseArea{
        id:mouseArea
        anchors.fill: parent
        onClicked: {
            checked = !checked;
            buttonTop.forceActiveFocus()
        }
    }
}
