import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import "View"

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")


    NavigtionBar {
        id: navBar
        anchors.left: parent.left
    }


    Rectangle {
        id: container
        anchors.left: navBar.right
        anchors.right: parent.right
        height: parent.height
        Text {
            id: content
            text: qsTr("1")
        }
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
        content.text = index
    }

    Component.onCompleted: {
        navBar.currentNavIndexChanged.connect(onCurrentNavIndexChanged)
    }
}
