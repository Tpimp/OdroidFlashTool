import QtQuick 2.6

Rectangle {
    id:topRect
    property alias crumbView:crumbView
    property string crumbPath:""
    function appendCrumb(crumb_text, crumb_data)
    {
        if(crumb_text.length > 15)
            crumbList.append({"ctext":"...","cdata":crumb_data});
        else
            crumbList.append({"ctext":crumb_text,"cdata":crumb_data});
        if(topRect.crumbPath.length > 0)
            topRect.crumbPath += ('/'+ crumb_text );
        else
            topRect.crumbPath += (crumb_text );
        crumbView.incrementCurrentIndex()
    }
    function itemCount()
    {
        return crumbList.count
    }

    function popLast()
    {
        if(crumbList.count > 0)
        {
            crumbList.remove(crumbList.count-1) // remove at last index
            crumbPath = "";
            var item_count = crumbList.count
            if(item_count > 0)
            {
                crumbPath += crumbList[0];
                var count = 1;
                for(count; count < item_count;)
                {
                    crumbPath += ('/' + crumbList[count]);
                    count++;
                }
            }
        }
        crumbView.decrementCurrentIndex()
    }

    function moveToCrumb(index)
    {
        var items_to_remove = (crumbList.count - index)-1;
        for(var count = 0; count < items_to_remove;)
        {
            popLast()
            count++;
        }
    }
    function clearCrumbs()
    {
        crumbPath = "";
        crumbList.clear()
    }

    ListModel{
        id:crumbList
    }
    Rectangle{
        anchors.fill: parent
        anchors.leftMargin: 4
        anchors.rightMargin:4
        anchors.topMargin:2
        anchors.bottomMargin:2
        color:topRect.color
        clip:true
        ListView{
            id:crumbView
            spacing:4
            anchors.fill: parent
            model:crumbList
            snapMode:ListView.SnapToItem
            orientation: Qt.Horizontal
            highlightFollowsCurrentItem : true
            add: Transition {
                NumberAnimation { properties: "x"; from: crumbView.width+75; duration: 400;}
            }
        }
    }


}
