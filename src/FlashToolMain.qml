import QtQuick 2.6

Rectangle {
    id:flashToolMain
    property string processText:""
    signal progressChanged(int progress)
    property bool   processRunning:false
    signal progressStart()

    function startOperation(operation,  data )
    {
        if(operation === "Finish")
        {
            operationStarted(operation,data);
            flashButton.enabled = true

        }
        else if(operation === "Compress")
        {
            ODF.startCompression(data);
            var  out_info = new Array;
            out_info[0] = deviceView.getSelectedDevice();
            out_info[1] = data;
            operationStarted(deviceView,out_info);
        }
    }
    signal operationStarted(string operation,var data);
    signal operationFinished(string operation, var data);
    Connections{
        target:ODF
        onDecompressionComplete:{
            progressBar.value = percentage
            progressText.text = processText + percentage.toString() + "%"
        }
        onCompressionCompleted:{
            progressBar.value = percentage
            progressText.text = processText + percentage.toString() + "%"
        }
        onReadCompleted:{
            progressBar.value = percentage
            progressText.text = processText + percentage.toString() + "%  @ " + read_speed;
            waitTime.text = "Estimated Wait: " + time_left;
        }
        onWriteCompleted:{
            progressBar.value = percentage
            progressText.text = processText + percentage.toString() + "%  @ " + write_speed;
            waitTime.text = "Estimated Wait: " + time_left;
        }
        onProcessStarted:{
            switch(id)
            {
                case 0: flashToolMain.processText = "Decompressing image: ";flashButton.buttonText.text = "Cancel Decompression"; break;
                case 1: flashToolMain.processText = "Compressing Image: ";flashButton.buttonText.text = "Cancel Compression"; break;
                case 2: flashToolMain.processText = "Writing Image: ";flashButton.buttonText.text = "Cancel Write"; break;
                case 3: flashToolMain.processText = "Reading Image: ";flashButton.buttonText.text = "Cancel Read"; break;
                default:break;
            }
            processRunning = true;

        }
        onProcessFinished:
        {
            flashButton.enabled = true;
            imageView.compressCheck.enabled = true;
            waitTime.visible = false;
            switch(id)
            {
                case 0:operationFinished("Writing", data); break;
                case 1: flashToolMain.processText = "Compressing Image: "; break;
                case 2: flashToolMain.processText = "Writing Image: "; break;
                case 3: operationFinished("Reading", data);   break;
                default:break;
            }
            processRunning = false;
        }
        onOperationCanceled:{
            if(flashMode)
            {
                if(imageView.shouldCompress)
                {
                    flashButton.buttonText.text = "Write Image"
                    progressText.text = "Ready to Write"
                }
            }
            else
            {
                flashButton.buttonText.text = "Read Image"
                progressText.text = "Ready to Read "
            }
        }

    }
    ProgressView{
        id:progressView
        anchors.left:deviceView.left
        anchors.right:deviceView.right
        anchors.bottom: flashButton.top
        anchors.top:deviceView.bottom
        anchors.bottomMargin:8
        anchors.topMargin:4
        onProgressUpdated: progressChanged(progress);
    }



    Button{
        id:flashButton
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom:parent.bottom
        anchors.bottomMargin: 6
        buttonText:Text{
            anchors.fill: parent
            text:  "Write Image"
            color:"white"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: parent.height * .55
            Component.onCompleted: parent = flashButton
        }
        color:"#6aa84f"
        border.color: "white"
        border.width: 2
        radius:12
        height:42
        width: 240
        mouseArea.hoverEnabled: true
        mouseArea.onHoveredChanged: {
            if(mouseArea.containsMouse)
            {
                flashButton.color = "#999999"
                flashButton.border.color = "#00ff00"
            }
            else
            {
                flashButton.color = "#6aa84f"
                flashButton.border.color = "white"
            }
        }
        mouseArea.onClicked: {
            if(processRunning)
            {
                ODF.cancelCurrentOperations()

            }
            else
            {
                imageView.compressCheck.enabled = false;
                if(imageView.flashMode) // write mode
                {
                    if(imageView.shouldCompress && imageView.filePath.length > 0)
                    {
                        ODF.startDecompression(imageView.filePath)
                    }
                    else
                    {
                        ODF.startImageWrite(deviceView.getSelectedDevice(), imageView.filePath,false,false)
                    }
                }
                else
                {
                    if(imageView.filePath.length == 0)
                        return;
                    ODF.startReadImage(deviceView.getSelectedDevice(), imageView.filePath,false,true)
                }
            }
        }
    }
    ImageFileView{
        id:imageView
        height:parent.height *.24
        anchors.left: parent.left
        anchors.topMargin: 2
        anchors.right: parent.right
        anchors.top: parent.top
        onFlashModeChanged: {
            if(flashMode)
            {
                flashButton.buttonText.text = "Write Image"
                progressView.progressText = "Ready to Write"
            }
            else
            {
                flashButton.buttonText.text = "Read Image"
                progressView.progressText = "Ready to Read"
            }
        }
        onFileBrowserOpen: {
            fileBrowserLoader.active = true
        }
    }


    DeviceView{
        id:deviceView
        height:parent.height *.28
        anchors.topMargin: 4
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: imageView.bottom
    }
    Loader{
        id:fileBrowserLoader
        anchors.fill: parent
        sourceComponent:FileBrowser{
            id:fileBrowser
            visible:false
            width:flashToolMain.width
            height:flashToolMain.height
            x:parent.width
            y:0
            onFileSelected: {
                imageView.filePath = file
                imageView.shouldCompress = isArchive
            }
            Component.onCompleted:
            {
                if(imageView.flashMode)
                    fileBrowser.openFileBrowser(0,ODF.getSystemRootUrl(),ODF.getSystemRootName())
                else
                    fileBrowser.openFileBrowser(1,ODF.getSystemRootUrl(),ODF.getSystemRootName())
            }
            onExited:
            {
                fileBrowserLoader.active = false
            }
        }
        active:false
    }
}
