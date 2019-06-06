import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.3

ApplicationWindow {
    id: rootWindow
    visible: true
    width: 1850
    height: 850
    minimumHeight: height
    minimumWidth: width
    maximumWidth: width
    maximumHeight: height
    title: qsTr("Travel Simulation")

    Material.theme: Material.Light
    Material.accent: Material.Teal

    MainPage{
        id: stackView
        focus: true
        anchors.fill: parent
    }

    //Popup to show messages or warnings on the bottom postion of the screen
    Popup {
        id: popup
        property alias popMessage: message.text

        background: Rectangle {
            implicitWidth: rootWindow.width
            implicitHeight: 60
            color: Material.color(Material.Red)
        }
        y: (rootWindow.height - 60)
        modal: true
        focus: true
        closePolicy: Popup.CloseOnPressOutside
        Text {
            id: message
            anchors.centerIn: parent
            font.pointSize: 12
            color: "#ffffff"
        }
        onOpened: popupClose.start()
    }

    // Popup will be closed automatically in 2 seconds after its opened
    Timer {
        id: popupClose
        interval: 2000
        onTriggered: popup.close()
    }
}
