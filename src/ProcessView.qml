import QtQuick 2.6

Rectangle {
    id:topRect
    radius:8
    width:processModel.count * 98
    gradient: Gradient {
        GradientStop {
            position: 0.00;
            color: "#dddddd";
        }
        GradientStop {
            position: 0.60;
            color: "#979696";
        }
    }
    border.width: 2
    property var   colorsToChoose:["darkgreen","darkblue","darkred","darkmagenta"]
    function getColor()
    {
        if(colorsToChoose.length > 0)
            return colorsToChoose.pop();
        return "orange"
    }
    function appendItem(item_type)
    {
        var color = getColor();
        processModel.append({"itemType":item_type,"itemColor":color})
    }
    function insertItem(item_index,item_type)
    {
        if(item_index >= 0)
        {
            var color = getColor();
            processModel.insert(item_index,{"itemType":item_type,"itemColor":color})
        }
    }

    function removeItem(item_index)
    {
        if(item_index >= 0)
        {
            console.log("Remove at index ", item_index)
            colorsToChoose.push(processModel.get(item_index).itemColor)
            processModel.remove(item_index)
        }
    }

    function clearList()
    {
        var length = processModel.count;
        for(var index = 0; index < length;)
        {
            var item_at = processModel.get(0)
            colorsToChoose.push(item_at.itemColor)
            processModel.remove(0);
            index++;
        }
    }

    ListModel{
        id: processModel
    }
    ListView{
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        height:28
        width:parent.width
        anchors.margins: 1
        model:processModel
        orientation:ListView.Horizontal
        spacing:3
        delegate:Rectangle{
            width:96
            height:parent.height
            radius:6
            color: model.itemColor
            border.color: "black"
            border.width: 2
            anchors.bottom: parent.bottom
            Text{
                anchors.fill: parent
                color:"white"
                text:model.itemType
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 12
                font.bold: true
                font.family: "Cambria"
            }
            Image{
                id:arrowImage
                anchors.left: parent.left
                visible: (index != 0)
                height:18
                width:26
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: -.54 * width
                source:"/images/arrow_down.png"
                rotation:-90
                fillMode:Image.Stretch
            }
        }
    }

}
