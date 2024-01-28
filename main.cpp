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
    QApplication  app(argc, argv);
    // QString currentFileName {"blocks_0.ssd"};
    // QString currentFileName {"blocks.ssd"};
    // QString currentFileName {"just.rsd"};
    // QString currentFileName {"sinusoids.ssd"};
    QString currentFileName {"million.ssd"};

    QScopedPointer<AbstractDataReader> dataReader(new SsdDataReader(currentFileName));
    dataReader->readData();

    QQmlApplicationEngine engine;
    auto context = engine.rootContext();

    QSharedPointer<GraphModel> graphData(new GraphModel());
    graphData->addData(dataReader->measurements());

    QSharedPointer<QVXYModelMapper> seriesMapper(new QVXYModelMapper());
    seriesMapper->setModel(graphData.data());
    seriesMapper->setXColumn(0);
    seriesMapper->setYColumn(1);

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
