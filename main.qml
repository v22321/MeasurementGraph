import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtCharts 2.15

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Measurement graph")

    Rectangle {
        anchors.fill: parent
        color: "black"

        ChartView {
            id: chartView
            anchors.fill: parent

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
    }
}
