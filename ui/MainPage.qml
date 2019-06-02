import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.3
import QtLocation 5.12
import QtPositioning 5.12
import com.name1e5s.travel 1.0

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
        ListElement { text: qsTr("ZhengZhou") }
        ListElement { text: qsTr("ChangSha") }
        ListElement { text: qsTr("WuHan") }
        ListElement { text: qsTr("HarBin") }
        ListElement { text: qsTr("ChangChun") }
        ListElement { text: qsTr("ShenYang") }
        ListElement { text: qsTr("ChengDu") }
        ListElement { text: qsTr("KunMing") }
        ListElement { text: qsTr("GuiYang") }
        ListElement { text: qsTr("Xi'an") }
        ListElement { text: qsTr("LanZhou") }
        ListElement { text: qsTr("GuangZhou") }
        ListElement { text: qsTr("NanNing") }
        ListElement { text: qsTr("NanJing") }
        ListElement { text: qsTr("HangZhou") }
        ListElement { text: qsTr("FuZhou") }
        ListElement { text: qsTr("JiNan") }
        ListElement { text: qsTr("NanChang") }
        ListElement { text: qsTr("HeFei") }
        ListElement { text: qsTr("HuHeHaoTe") }
        ListElement { text: qsTr("LaSa") }
        ListElement { text: qsTr("Urumuqi") }
        ListElement { text: qsTr("YinChuan") }
        ListElement { text: qsTr("XiNing") }
        ListElement { text: qsTr("HaiKou") }
    }

    GraphHandler {
        id: graphHandler
    }

    Connections {
        target: graphHandler
        onTotalPriceChanged: {
            totalPriceText.text = "Price: " + graphHandler.totalPrice
        }

        onTotalTimeChanged: {
            totalTimeText.text = "Time: " + graphHandler.totalTime
        }
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

            Rectangle {
                height: 50
            }

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
                    onCurrentIndexChanged: {
                        graphHandler.sourceCity = currentIndex
                        checkmodel.setProperty(currentIndex ,"value", false)
                        listView.model
                    }
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
                    onCurrentIndexChanged: {
                        graphHandler.destCity = currentIndex
                        checkmodel.setProperty(currentIndex ,"value", false)
                        listView.update()
                    }
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
                    handleDateBox()
                }
            }



            Text {
                text: "SELECT CICIES"
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            }

            ListModel {
                id: checkmodel
                Component.onCompleted: {
                    for(var i = 0; i < 31; i++){
                        checkmodel.append({"name": cityList.get(i).text, "value": false})
                    }
                }
            }

            Rectangle {
                height: 10
            }

            ColumnLayout {
                ListView {
                    id: listView
                    model: checkmodel
                    width: parent.width
                    height: 200
                    delegate: CheckDelegate {
                        text: name + " ".repeat(20 - name.length) + "\t\t"
                        checked: value
                        enabled: !(index === fromCityComboBox.currentIndex || index === toCityComboBox.currentIndex)
                        onCheckStateChanged: checkmodel.setProperty(index ,"value", checked)
                    }
                    flickableDirection: Flickable.VerticalFlick

                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    ScrollBar.vertical: ScrollBar {}
                 }
            }

            Rectangle {
                height: 10
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
                    model: ["Least Cost", "Best Time", "Best Perf"]
                    onCurrentIndexChanged: {
                        graphHandler.planType = currentIndex
                    }
                }
            }

            RowLayout {
                Layout.preferredWidth: parent.width * 0.9
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                TextField {
                    id: hourBox
                    placeholderText: qsTr("Leave time ...")
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    inputMethodHints: Qt.ImhDigitsOnly
                    enabled: planTypeComboBox.currentIndex === 2
                    Keys.onReturnPressed: {
                        leaveTimeHandler()
                    }
                }

                TextField {
                    id: limitBox
                    placeholderText: qsTr("Time limit...")
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    inputMethodHints: Qt.ImhDigitsOnly
                    enabled: planTypeComboBox.currentIndex === 2
                    Keys.onReturnPressed: {
                        limitHandler()
                    }
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
                    onClicked: {
                        runPlan()
                    }
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

            RowLayout {
                Layout.preferredWidth: parent.width * 0.9
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                Text {
                    id: totalTimeText
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    width: parent.width * 0.4
                    font.pointSize: 14
                    text: qsTr("Time:")
                }

                Text {
                    id: totalPriceText
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    width: parent.width * 0.4
                    font.pointSize: 14
                    text: qsTr("Price:")
                }
            }

            Rectangle {
                height: 50
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

    function handleDateBox() {
        if(isValidDate(dateBox.text)) {
            graphHandler.beginYear = dateBox.text.slice(0,4)
            graphHandler.beginMonth = dateBox.text.slice(5,7)
            graphHandler.beginYear = dateBox.text.slice(8,10)
        } else {
            popup.popMessage = "Wrong Data!"
            popup.open()
            dateBox.clear()
        }
    }

    function isValidDate(dateString) {
      var regEx = /^\d{4}-\d{2}-\d{2}$/;
      if(!dateString.match(regEx)) return false;  // Invalid format
      var d = new Date(dateString);
      var dNum = d.getTime();
      if(!dNum && dNum !== 0) return false; // NaN value, Invalid date
      return d.toISOString().slice(0,10) === dateString;
    }

    function leaveTimeHandler() {
        var regEx = /^\d+$/;
        if(!hourBox.text.match(regEx) || parseInt(hourBox.text) > 23 || parseInt(hourBox.text) < 0) {
            popup.popMessage = "Wrong Leave Time!"
            popup.open()
            dateBox.clear()
        } else {
            graphHandler.leaveTime = hourBox.text
        }
    }

    function limitHandler() {
        var regEx = /^\d+$/;
        if(!limitBox.text.match(regEx)) {
            popup.popMessage = "Wrong Limit!"
            popup.open()
            dateBox.clear()
        } else {
            graphHandler.timeLimit = limitBox.text
        }
    }

    function runPlan() {
        handleDateBox()
        if(planTypeComboBox.currentIndex === 2) {
            leaveTimeHandler()
            limitHandler()
        }

        graphHandler.clearMiddleCity()
        for(var vn = 0 ;vn < 31; vn++) {
            if(checkmodel.get(vn).value) {
                graphHandler.appendMiddleCity(vn)
            }
        }
        console.log(graphHandler.middleCity)

        graphHandler.generateResult()
        console.log(graphHandler.citySequence)
        console.log(graphHandler.tranName)
    }
}
