import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.12


Rectangle {
    id: navButton
    width: 120
    height: 120
    color: "#ffffff"
    property var text: ""
    border.color: "gray"
    border.width: isClicked ? 0 : 1
    property int index: 0
    property bool isClicked: false
    signal clickedChanged(int index)

    function clear()
    {
        navButton.color = "#ffffff"
        navButton.isClicked = false
    }

    Text{
        anchors.centerIn: parent
        text: navButton.text
        color: "black"
    }

    MouseArea{
        anchors.fill: parent
        hoverEnabled: true
        propagateComposedEvents: true

        onClicked: {
            parent.color = "#98A9EE"
            navButton.clickedChanged(index);
            navButton.isClicked = true
        }

        onEntered: {
            if (isClicked)
            {
                parent.color = "#767C7C"
            }
        }

        onExited: {
            if (isClicked)
            {
                parent.color = "#ffffff"
            }
        }
    }
}

