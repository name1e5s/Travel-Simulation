import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.3
import QtLocation 5.12
import QtPositioning 5.12

Page {
    id: infoPage

    Material.theme: Material.Light
    Material.accent: Material.Teal
    width: parent.width
    height: parent.height

    // Common List Model for ComboBox
    ListModel {
        id: cityList
        ListElement { text: qsTr("Beijing") }
        ListElement { text: qsTr("Tianjin") }
        ListElement { text: qsTr("ShangHai") }
        ListElement { text: qsTr("ChongQing") }
        ListElement { text: qsTr("ShiJiaZhuang") }
        ListElement { text: qsTr("TaiYuan") }
        ListElement { text: qsTr("HuHeHaoTe") }
        ListElement { text: qsTr("ZhengZhou") }
        ListElement { text: qsTr("ChangSha") }
        ListElement { text: qsTr("WuHan") }
        ListElement { text: qsTr("HarBin") }
        ListElement { text: qsTr("ChangChun") }
        ListElement { text: qsTr("ShenYang") }
        ListElement { text: qsTr("ChengDu") }
        ListElement { text: qsTr("KunMing") }
        ListElement { text: qsTr("GuiYang") }
        ListElement { text: qsTr("LaSa") }
        ListElement { text: qsTr("Urumuqi") }
        ListElement { text: qsTr("Xi'an") }
        ListElement { text: qsTr("LanZhou") }
        ListElement { text: qsTr("YinChuan") }
        ListElement { text: qsTr("XiNing") }
        ListElement { text: qsTr("GuangZhou") }
        ListElement { text: qsTr("NanNing") }
        ListElement { text: qsTr("HaiKou") }
        ListElement { text: qsTr("NanJing") }
        ListElement { text: qsTr("HangZhou") }
        ListElement { text: qsTr("FuZhou") }
        ListElement { text: qsTr("JiNan") }
        ListElement { text: qsTr("NanChang") }
        ListElement { text: qsTr("HeFei") }
    }

    Plugin {
        id: mapPlugin
        name: "osm"
    }

    RowLayout {
        width: parent.width
        height: parent.height

        Map {
            Layout.preferredHeight: parent.height
            Layout.preferredWidth: parent.width * 0.8
            plugin: mapPlugin
            center: QtPositioning.coordinate(36.858,104.676)
            zoomLevel: 5

            MapPolyline {
                line.width: 2
                line.color: 'red'
                path: [
                    { latitude: 39.91667, longitude: 116.41667 },
                    { latitude: 35.03000, longitude: 110.97000 },
                    { latitude: 34.26667, longitude: 108.95000 }
                ]
            }
        }

        ColumnLayout {
            Layout.preferredWidth: parent.width * 0.2
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

            RowLayout {
                Layout.preferredWidth: parent.width * 0.9
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                Text {
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    width: parent.width * 0.1
                    text: qsTr("From:")
                }

                ComboBox {
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    width: parent.width * 0.4
                    id: fromCityComboBox
                    currentIndex: 0
                    model: cityList
                }

                Text {
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    width: parent.width * 0.1
                    text: qsTr("To:")
                }

                ComboBox {
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    width: parent.width * 0.4
                    id: toCityComboBox
                    currentIndex: 0
                    model: cityList
                }

            }

            Text {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                width: parent.width * 0.1
                text: qsTr("LEAVE DATE")
            }

            TextField {
                id: dateBox
                placeholderText: qsTr("YYYY-MM-DD here ...")
                Layout.preferredWidth: parent.width * 0.9
                Layout.alignment: Qt.AlignHCenter

                font.pointSize: 14
                font.family: "fontawesome"
                leftPadding: 40
                background: Rectangle {
                    implicitWidth: 200
                    implicitHeight: 50
                    radius: implicitHeight / 2
                    color: "transparent"
                    Text {
                        text: "\uf073"
                        font.pointSize: 13
                        font.bold: true
                        font.family: "fontawesome"
                        color: Material.accent
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        leftPadding: 10
                    }

                    Rectangle {
                        width: parent.width - 10
                        height: 1
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.bottom: parent.bottom
                        color: Material.accent
                    }
                }
                Keys.onReturnPressed: {
                    if(isValidDate(dateBox.text)) {

                    } else {
                        popup.popMessage = "Wrong Data!"
                        popup.open()
                        dateBox.clear()
                    }
                }
            }



            Text {
                text: "SELECT CICIES"
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            }

            GridLayout {
                id: wayPointGridLayout
                columns: 4
                Layout.preferredWidth: parent.width * 0.9
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                Repeater {
                    model: 31
                    Layout.preferredWidth: parent.width
                    CheckBox {
                        Layout.row: index / 4
                        Layout.column: index % 4
                        Layout.fillWidth: true
                        text: cityList.get(index).text
                        checked: false
                    }
                }
            }

            RowLayout {
                Layout.preferredWidth: parent.width * 0.9

                Text {
                    width: parent.width * 0.3
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    text: "Plan Type: "
                }

                ComboBox {
                    Layout.alignment: Qt.AlignHCenter
                    id: planTypeComboBox
                    Component.onCompleted: width = calcComboBoxImplicitWidth(planTypeComboBox)
                    currentIndex: 0
                    model: ["Best Timesdsds", "Least Cost", "Best Perf"]
                }
            }

            Rectangle {
                Layout.preferredWidth: parent.width * 0.9
                Layout.preferredHeight: 40
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                color: "black"
                border.color: Material.accent

                Text {
                    anchors.centerIn: parent
                    text: "Log Here"
                    font.pointSize: 18
                    color: "#18f018"
                }
            }

            RowLayout {
                Layout.preferredWidth: parent.width * 0.9
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                Button {
                    id: runPlanButton
                    width: parent.width * 0.2
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    text: "\uf04b"
                    font.pointSize: 14
                    font.family: "fontawesome"
                    hoverEnabled: true
                    ToolTip.delay: 1000
                    ToolTip.timeout: 5000
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Do Plan.")
                }

                Button {
                    id: runSimuButton
                    width: parent.width * 0.2
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    text: "\uf5b0"
                    font.pointSize: 14
                    font.family: "fontawesome"
                    hoverEnabled: true
                    ToolTip.delay: 1000
                    ToolTip.timeout: 5000
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Run Simulation.")
                }

                Button {
                    id: saveLogButton
                    width: parent.width * 0.2
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    text: "\uf019"
                    font.pointSize: 14
                    font.family: "fontawesome"
                    hoverEnabled: true
                    ToolTip.delay: 1000
                    ToolTip.timeout: 5000
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Save Log.")
                }

            }
        }

    }

    function calcComboBoxImplicitWidth(cb) {
      var widest = 0
      if (cb.count===0) return cb.width
      var originalCI = cb.currentIndex
      if (originalCI < 0) return cb.width // currentIndex →  deleted item
      do {
        widest = Math.max(widest, cb.contentItem.contentWidth)
        cb.currentIndex = (cb.currentIndex + 1) % cb.count
      } while(cb.currentIndex !== originalCI)

      return widest + cb.contentItem.leftPadding + cb.contentItem.rightPadding
                    + cb.indicator.width
    }

    function isValidDate(dateString) {
      var regEx = /^\d{4}-\d{2}-\d{2}$/;
      if(!dateString.match(regEx)) return false;  // Invalid format
      var d = new Date(dateString);
      var dNum = d.getTime();
      if(!dNum && dNum !== 0) return false; // NaN value, Invalid date
      return d.toISOString().slice(0,10) === dateString;
    }

}
