import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import "View"

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hawk")
    property var widget: null

    NavigtionBar {
        id: navBar
        anchors.left: parent.left
    }


    Rectangle {
        id: container
        anchors.left: navBar.right
        anchors.right: parent.right
        height: parent.height
    }

//    SwipeView {
//        id: viewContainer
//        anchors.left: navBar.right
//        anchors.right: parent.right
//        height: parent.height
//        currentIndex: navBar.currentIndex
//        Rectangle {
//            id: view1
//            color: "#000000"
//        }

//        Rectangle {
//            id: view2
//            color: "#ffffff"
//        }
//    }

    function onCurrentNavIndexChanged(index) {
        console.log(index)
        var viewPath = ""
        switch(index)
        {
        case 0: //upload
            viewPath = "qrc:/View/UploadView.qml"
            break
        case 1: //gallery
            viewPath = "qrc:/View/GalleryView.qml"
            break
        case 2: //setup
            viewPath = "qrc:/View/SetupView.qml"
            break
        case 3: //about
            viewPath = "qrc:/View/AboutView.qml"
            break
        }
        if (viewPath !== "")
        {
            var component = Qt.createComponent(viewPath)
            if (widget != null)
            {
                widget.destroy()
            }
            widget = component.createObject(container)
        }


    }

    Component.onCompleted: {
        navBar.currentNavIndexChanged.connect(onCurrentNavIndexChanged)
    }
}
