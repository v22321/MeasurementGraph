import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15
import QtCharts 2.15

Window {
    id: window
    width: 900
    height: 600
    visible: true
    // visibility: Window.FullScreen
    title: qsTr("Measurement graph")

    property bool hasError: false

    Connections {
        target: wrapper

        function onS_hasError() {
            hasError = true
        }
    }

    ColumnLayout {
        anchors.fill: parent
        Rectangle {
            id: errorMessageRect
            Layout.fillWidth: true
            Layout.preferredHeight: 60
            border {
                width: 2
                color: hasError ? "red" : "black"
            }
            radius: 4

            Row {
                anchors {
                    fill: parent
                    margins: 24
                }
                spacing: 12


                Label {
                    id: selectFileLabel
                    text: "Select file: "
                    font.pixelSize: 14
                    textFormat: Text.RichText
                    anchors.verticalCenter: parent.verticalCenter
                }

                ComboBox {
                    id: selectFileCombo
                    anchors.verticalCenter: parent.verticalCenter
                    model: Qt.isQtObject(wrapper) ? wrapper.fileNames : "-"
                }

                Button {
                    id: selectFileBtn
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Make graph"

                    onClicked: {
                        hasError = false
                        wrapper.s_createGraph(selectFileCombo.currentValue)
                    }
                }

                Label {
                    id: errorMsg
                    text: "Error: Input file has wrong format"
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 14
                    color: "red"
                    textFormat: Text.RichText
                    visible: hasError
                }

            }
        }

        ChartView {
            id: chartView
            Layout.fillHeight: true
            Layout.fillWidth: true
            // anchors.fill: parent

            Component.onCompleted: {
                if (Qt.isQtObject(seriesMapper))
                    seriesMapper.series = lineSeries
            }

            LineSeries {
                id: lineSeries
                name: "Measurement results"
                axisX: customAxisX
                axisY: customAxisY
            }

            ValueAxis {
                id: customAxisX
                tickCount: 10
                min: Qt.isQtObject(graphData) ? graphData.minX : 0
                max: Qt.isQtObject(graphData) ? graphData.maxX : 0
            }

            ValueAxis {
                id: customAxisY
                tickCount: 10
                min: Qt.isQtObject(graphData) ? graphData.minY : 0
                max: Qt.isQtObject(graphData) ? graphData.maxY : 0
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: window.height / 4
            color: "lightgray"

            Label {
                anchors {
                    fill: parent
                    margins: 12
                }
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignTop
                text: Qt.isQtObject(graphData) ? graphData.measurementInfo : "No information"
                font.pixelSize: 14
                textFormat: Text.RichText
            }
        }
    }
}
