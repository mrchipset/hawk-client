import QtQuick 2.0
import QtQuick.Dialogs 1.2

Item {
    id: uploadView
    anchors.fill: parent
    Column {
        anchors.fill: parent
        Image {
            id: uploadImage
            width: parent.width
            height: parent.height * 9 / 10
            source: "qrc:/Resource/white.png"
            fillMode: Image.Stretch
            cache: false
            MouseArea {
                anchors.fill: parent
                onClicked: function () {
                    fileDialog.open()
                }
            }
        }

        Text {
            id: response
            height: parent.height / 10
            text: qsTr("text")
        }
    }

    FileDialog {
        id: fileDialog
        title: qsTr("Please choose an image file")
        nameFilters: ["Image Files (*.jpg *.png *.gif *.bmp *.ico)", "*.*"]
        onAccepted: {
            uploadImage.source = fileUrl
            hawkClient.postPicture(fileUrl)
            console.log(hawkClient.error())
        }
    }

}
