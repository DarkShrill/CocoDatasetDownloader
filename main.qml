import QtQuick 2.10
import QtQuick.Window 2.10
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.3

import it.videx.dataset 1.0
import it.videx.progressbar 1.0

Window {
    id: mainWindow
    width: 640
    height: 480
    visible: true
    title: qsTr("Dataset Downloader")


    FontLoader {
        id: fontLoaderGothamMedium
        source: "qrc:/other/fonts/GothamMedium.ttf"
    }

    FontLoader {
        id: fontLoaderGothamBook
        source: "qrc:/other/fonts/GothamBook.ttf"
    }

    FontLoader {
        id: fontLoaderGothamLight
        source: "qrc:/other/fonts/GothamLight.ttf"
    }

    Button{
        id: fileDialogJsonDatasetButton

        font.family: fontLoaderGothamBook.name
        font.pixelSize: 14

        width: parent.width - 50
        anchors.left: parent.left
        anchors.leftMargin: 25
        anchors.top: parent.top
        anchors.topMargin: 8
        height: 48

        text: "Select Json Dataset"

        FileDialog{
            id: fileDialogJsonDataset
            folder: shortcuts.home
            selectFolder:false

            onAccepted: {
                console.log("You chose: " + fileDialogJsonDataset.fileUrls)
                loading.show();

                labelJsonDataset.text = fileDialogJsonDataset.fileUrl.toString().replace(/^file:\/{3}/,"");

                DatasetCreatorConnector.setDatasetJsonFile(fileDialogJsonDataset.fileUrl.toString().replace(/^file:\/{3}/,""))

            }
        }

        onClicked: {
            fileDialogSaveFolderButton.visible = false;
            fileDialogJsonDataset.visible = true;
        }
    }

    Label{
        id:labelJsonDataset
        anchors.top: fileDialogJsonDatasetButton.bottom
        anchors.topMargin: 8
        anchors.horizontalCenter: parent.horizontalCenter
        font.family: fontLoaderGothamBook.name
        font.pointSize: 7
        font.bold: true
    }



    Window{
        id:loading

        width: mainWindow.width/3
        height: mainWindow.height/3

        modality: Qt.ApplicationModal

        flags: Qt.SplashScreen

        visible: false

        color: "#005EB8"


        BlockLoader{
            id:loader
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.top: parent.top
            anchors.topMargin: 15
            width: parent.width - 75
            height: parent.height - 75
            visible: loading.visible
        }

        Label{
            text: "Loading.."
            anchors.top: loader.bottom
            anchors.topMargin: 32
            anchors.horizontalCenter: parent.horizontalCenter
            color: "white"
            font.family: fontLoaderGothamBook.name

        }

        function show(){
//            loading.visible = true;
//            loader.startAnimation();
        }

        function hide(){
            loading.visible = false;
//            loader.stopAnimation();
        }
    }

    Connections{
        target: DatasetCreatorConnector

        onJsonLoaded:{
            loading.hide();
            fileDialogSaveFolderButton.visible = true;
        }

        onSendCategories:{
            categoryCheckbox.model = categories;
        }

        onProcessedImage:{
            if(progressBar.visible == false){
                progressBar.visible = true;
                fadeIn.start();
            }

            progressBar.value = parseFloat(processed_images)
        }
    }





    Button{
        id: fileDialogSaveFolderButton

        visible: false

        font.family: fontLoaderGothamBook.name
        font.pixelSize: 14

        width: parent.width - 50
        anchors.left: parent.left
        anchors.leftMargin: 25
        anchors.top: labelJsonDataset.bottom
        anchors.topMargin: 32
        height: 48

        text: "Select Save Folder"

        FileDialog{
            id: fileDialogSaveFolder
            folder: shortcuts.home
            selectFolder:true

            onAccepted: {
                console.log("You chose: " + fileDialogSaveFolder.fileUrls)

                labelSaveFolder.text = fileDialogSaveFolder.fileUrl.toString().replace(/^file:\/{3}/,"");

                DatasetCreatorConnector.setDatasetFolder(fileDialogSaveFolder.fileUrl.toString().replace(/^file:\/{3}/,""))

                categoryCheckbox.visible = true;
                batchCheckbox.visible = true;
                startButton.visible = true;

            }
        }

        onClicked: {
            fileDialogSaveFolder.visible = true;
        }
    }

    Label{
        id:labelSaveFolder
        anchors.top: fileDialogSaveFolderButton.bottom
        anchors.topMargin: 8
        anchors.horizontalCenter: parent.horizontalCenter
        font.family: fontLoaderGothamBook.name
        font.pointSize: 7
        font.bold: true
    }

    ComboBox{
        id:categoryCheckbox

        width: parent.width - 50
        height:48
        anchors.left: parent.left
        anchors.leftMargin: 25
        anchors.top: labelSaveFolder.bottom
        anchors.topMargin: 32

        visible: false



        contentItem: Text {
            text: parent.displayText
            font.family: fontLoaderGothamBook.name
            font.pixelSize: 14
            verticalAlignment: Text.AlignVCenter;
            horizontalAlignment: Text.AlignHCenter;
            elide: Text.ElideRight
        }

        onCurrentTextChanged: {
            if(currentText != ""){
                if(batchCheckbox.currentText != ""){
                    startButton.enabled = true;
                }
                DatasetCreatorConnector.setCategoryId(parseInt(currentIndex))
            }
        }

    }


    ComboBox{
        id:batchCheckbox

        width: parent.width - 50
        height:48
        anchors.left: parent.left
        anchors.leftMargin: 25
        anchors.top: categoryCheckbox.bottom
        anchors.topMargin: 32

        visible: false

        model:["", "1","10","50","100"]


        contentItem: Text {
            text: parent.displayText
            font.family: fontLoaderGothamBook.name
            font.pixelSize: 14
            verticalAlignment: Text.AlignVCenter;
            horizontalAlignment: Text.AlignHCenter;
            elide: Text.ElideRight
        }

        onCurrentTextChanged: {
            if(currentText != ""){
                if(categoryCheckbox.currentText != ""){
                    startButton.enabled = true;
                }
                DatasetCreatorConnector.setBatchSize(parseInt(currentText))
            }
        }
    }



    Button{
        id: startButton

        visible: false

        enabled: false

        font.family: fontLoaderGothamBook.name
        font.pixelSize: 18
        font.bold: true

        width: parent.width - 50
        anchors.left: parent.left
        anchors.leftMargin: 25
        anchors.top: batchCheckbox.bottom
        anchors.topMargin: 64
        height: 48

        text: "Start"

        onClicked: {

            startButton.visible = false;
            categoryCheckbox.visible = false;
            batchCheckbox.visible = false;
            fileDialogJsonDatasetButton.visible = false;
            labelJsonDataset.visible = false;
            fileDialogSaveFolderButton.visible = false;
            labelSaveFolder.visible = false;

            progressBar.visible = true;
            progressBar.value = 0;
            fadeIn.start();

            DatasetCreatorConnector.startDownload()
        }
    }





    CustomProgressBar{
        property var colorGradient : "#005EB8"//Qt.rgba(1,1,1,1)

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
//        anchors.top: fileDialog.bottom
//        anchors.topMargin: 16

        id:progressBar
        width: 150
        height: 150
        penStyle: Qt.RoundCap
        dialType: CustomProgressBar.MinToMax
        progressColor: colorGradient//"#c61e5d"
        foregroundColor: "#C9C9C9"
        dialWidth: 10
        startAngle: 45
        spanAngle: 270
        minValue: 0
        maxValue: 100
        value: 0
        //scale:0
        textFont {
            family: fontLoaderGothamBook.name
            italic: false
            pointSize: 16
        }
        suffixText: "%\nProgress"
        textColor: colorGradient

        visible: false

        SequentialAnimation {
            id:successAnimation
            running: false
            ColorAnimation{
                target: progressBar
                properties: "colorGradient"
                from:"#005EB8"
                to:"#77DD77"
                duration: 1200
            }

            PauseAnimation {
                duration: 500
            }

            PropertyAnimation{
                target: progressBar
                properties: "opacity"
                from:1
                to: 0
                duration: 350
                easing.type: Easing.InCirc
            }
        }

        ParallelAnimation{
            id:fadeIn
            running: false
            PropertyAnimation{
                id:opacityAnimation
                target: progressBar
                properties: "opacity"
                from:0
                to: 1
                duration: 350
                easing.type: Easing.InCirc
            }

            PropertyAnimation{
                id:scaleAnimation
                target: progressBar
                properties: "scale"
                from:0
                to: 1
                duration: 200
                easing.type: Easing.InCirc
            }
        }
    }

}
