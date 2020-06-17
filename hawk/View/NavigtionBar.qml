import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Rectangle {
    id: navBar
    anchors.left: parent.left
    width: 120
    color: "#1B96D1"
    property int acc: 0
    property int currentIndex: 0
    signal currentNavIndexChanged(int index)
    ColumnLayout {
        id: layout
        width: parent.width
        Layout.fillHeight: true
        spacing: 1
    }

    function addNavButton(text, index)
    {
        var component = Qt.createComponent("NavigationButton.qml")
        if (component.status === Component.Ready)
        {
            var navButton = component.createObject(layout)
            navButton.text = text
            navButton.index = index;
            navButton.clickedChanged.connect(onClickedChanged(navButton))
        }
    }

    function onClickedChanged(sender)
    {
        return function(index) {
            currentIndex = index
            navBar.currentNavIndexChanged(index)
            for(var i in layout.children)
            {
                layout.children[i].clear();
            }
        }
    }

    Component.onCompleted: {
        addNavButton("Upload", 0)
        addNavButton("Gallery", 1)
        addNavButton("Setup", 2)
        addNavButton("About", 3)
    }
}



