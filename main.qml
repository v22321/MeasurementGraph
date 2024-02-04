import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15
import MyNamespace 1.0
// import QtCharts 2.15

Window {
    id: window
    width: 900
    height: 600
    visible: true
    // visibility: Window.FullScreen
    title: qsTr("Measurement graph")

    property bool hasError: false
    readonly property int currentPixelSize: 14
    readonly property int desktopMargin: 24

    Connections {
        target: wrapper

        function onS_hasError() {
            hasError = true
            selectFileBtn.enabled = true
        }

        function onS_graphUpdated(el) {
            // console.log("Graph updated")
            // selectFileBtn.enabled = true
        }
    }

    ColumnLayout {
        anchors.fill: parent
        Rectangle {
            id: errorMessageRect
            Layout.fillWidth: true
            Layout.preferredHeight: window.height / 10
            border {
                width: 2
                color: hasError ? "red" : "black"
            }
            radius: 4

            Row {
                anchors {
                    fill: parent
                    margins: desktopMargin
                }
                spacing: desktopMargin / 2


                Label {
                    id: selectFileLabel
                    text: qsTr("Select file") + ": "
                    font.pixelSize: currentPixelSize
                    textFormat: Text.RichText
                    anchors.verticalCenter: parent.verticalCenter
                }

                ComboBox {
                    id: selectFileCombo
                    anchors.verticalCenter: parent.verticalCenter
                    model: Qt.isQtObject(wrapper) ? wrapper.dataFiles : "-"
                    enabled: selectFileBtn.enabled
                }

                Button {
                    id: selectFileBtn
                    anchors.verticalCenter: parent.verticalCenter
                    text: qsTr("Create graph")

                    onClicked: {
                        if (Qt.isQtObject(wrapper))
                        {
                            selectFileBtn.enabled = false
                            hasError = false
                            wrapper.s_createGraph(selectFileCombo.currentValue)
                        }
                        else
                            hasError = true
                    }
                }

                Label {
                    id: errorMsg
                    text: qsTr("Error: Input file has wrong format")
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: currentPixelSize
                    color: "red"
                    textFormat: Text.RichText
                    visible: hasError
                }
            }
        }

        GraphPainter {
            id: graph
            Layout.fillHeight: true
            Layout.fillWidth: true

            onS_graphUpdated: {
                console.log("Graph updated")
                selectFileBtn.enabled = true
            }

            Connections {
                target: wrapper

                function onS_graphUpdated(points)
                {
                    graph.updateData(points);
                }

                function onS_setMaxMinPoints(maxMinValues)
                {
                    graph.setMaxMinValues(maxMinValues);
                }
            }
        }
        // ChartView {
        //     id: chartView
        //     Layout.fillHeight: true
        //     Layout.fillWidth: true

        //     Component.onCompleted: {
        //         if (Qt.isQtObject(seriesMapper))
        //             seriesMapper.series = lineSeries
        //     }

        //     LineSeries {
        //         id: lineSeries
        //         name: qsTr("Measurement results")
        //         axisX: customAxisX
        //         axisY: customAxisY
        //     }

        //     ValueAxis {
        //         id: customAxisX
        //         tickCount: 10
        //         min: Qt.isQtObject(graphData) ? graphData.minX : 0
        //         max: Qt.isQtObject(graphData) ? graphData.maxX : 0
        //     }

        //     ValueAxis {
        //         id: customAxisY
        //         tickCount: 10
        //         min: Qt.isQtObject(graphData) ? graphData.minY : 0
        //         max: Qt.isQtObject(graphData) ? graphData.maxY : 0
        //     }
        // }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: window.height / 4
            color: "lightgray"

            Label {
                anchors {
                    fill: parent
                    margins: desktopMargin / 2
                }
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignTop
                text: Qt.isQtObject(graphData) ? graphData.measurementInfo : qsTr("No information")
                font.pixelSize: currentPixelSize
                textFormat: Text.RichText
            }
        }
    }
}
