#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtCharts/QVXYModelMapper>

#include <QSharedPointer>
#include <QDebug>

#include "data_parser/ssddatareader.h"
#include "graph/graphmodel.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication app(argc, argv);
    // const QString currentFileName { "SampleFiles/blocks_0.ssd" };
    // QString currentFileName { "SampleFiles/blocks.ssd" };
    // QString currentFileName { "SampleFiles/just.rsd" };
    // QString currentFileName { "SampleFiles/sinusoids.ssd" };
    QString currentFileName { "SampleFiles/million.ssd" };

    bool hasError {true};

    /// Get measurement data
    QSharedPointer<AbstractDataReader> dataReader(new SsdDataReader(currentFileName));
    hasError = dataReader->readData();
    if (hasError)
    {
        qWarning() << "Can't read file. Stop";
        return -1;
    }

    const auto& measurements { dataReader->measurements() };
    if (measurements.size() == 0)
    {
        qWarning() << "Empty measurements";
        return -1;
    }

    const auto& firstElement { measurements.at(0) };
    /// Set graph data
    QSharedPointer<GraphModel> graphData(new GraphModel(firstElement));
    graphData->addPoints(dataReader->measurements());

    QSharedPointer<QVXYModelMapper> seriesMapper(new QVXYModelMapper());
    seriesMapper->setModel(graphData.data());
    seriesMapper->setXColumn(0);
    seriesMapper->setYColumn(1);

    QQmlApplicationEngine engine;
    auto context = engine.rootContext();

    context->setContextProperty("seriesMapper", seriesMapper.data());
    context->setContextProperty("graphData", graphData.data());

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
