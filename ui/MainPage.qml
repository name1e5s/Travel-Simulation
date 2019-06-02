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
        ListElement { text: qsTr("北京") }
        ListElement { text: qsTr("天津") }
        ListElement { text: qsTr("上海") }
        ListElement { text: qsTr("重庆") }
        ListElement { text: qsTr("石家庄") }
        ListElement { text: qsTr("太原") }
        ListElement { text: qsTr("郑州") }
        ListElement { text: qsTr("长沙") }
        ListElement { text: qsTr("武汉") }
        ListElement { text: qsTr("哈尔滨") }
        ListElement { text: qsTr("长春") }
        ListElement { text: qsTr("沈阳") }
        ListElement { text: qsTr("成都") }
        ListElement { text: qsTr("昆明") }
        ListElement { text: qsTr("贵阳") }
        ListElement { text: qsTr("西安") }
        ListElement { text: qsTr("兰州") }
        ListElement { text: qsTr("广州") }
        ListElement { text: qsTr("南宁") }
        ListElement { text: qsTr("南京") }
        ListElement { text: qsTr("杭州") }
        ListElement { text: qsTr("福州") }
        ListElement { text: qsTr("济南") }
        ListElement { text: qsTr("南昌") }
        ListElement { text: qsTr("合肥") }
        ListElement { text: qsTr("呼和浩特") }
        ListElement { text: qsTr("拉萨") }
        ListElement { text: qsTr("乌鲁木齐") }
        ListElement { text: qsTr("银川") }
        ListElement { text: qsTr("西宁") }
        ListElement { text: qsTr("海口") }
    }

    GraphHandler {
        id: graphHandler
    }

    Connections {
        target: graphHandler
        onTotalPriceChanged: {
            totalPriceText.text = "总价格: " + graphHandler.totalPrice
        }

        onTotalTimeChanged: {
            totalTimeText.text = "总时间: " + graphHandler.totalTime
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
                    text: qsTr("始发:")
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
                    text: qsTr("终到:")
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
                text: qsTr("出发日期")
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
                text: "途径城市"
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
                    text: "策略类型: "
                }

                ComboBox {
                    Layout.alignment: Qt.AlignHCenter
                    id: planTypeComboBox
                    Component.onCompleted: width = calcComboBoxImplicitWidth(planTypeComboBox)
                    currentIndex: 0
                    model: ["最低价格", "最短时间", "规定时间内最少价格"]
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
                    placeholderText: qsTr("出发时间(小时) ...")
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    inputMethodHints: Qt.ImhDigitsOnly
                    enabled: planTypeComboBox.currentIndex === 2
                    Keys.onReturnPressed: {
                        leaveTimeHandler()
                    }
                }

                TextField {
                    id: limitBox
                    placeholderText: qsTr("时间限制(小时)...")
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
                    text: "这里是日志"
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
                    ToolTip.text: qsTr("开始规划")
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
                    ToolTip.text: qsTr("模拟旅行")
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
                    ToolTip.text: qsTr("保存日志")
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
                    text: qsTr("总时间: 0")
                }

                Text {
                    id: totalPriceText
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    width: parent.width * 0.4
                    font.pointSize: 14
                    text: qsTr("总价格: 0")
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
