#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtCharts/QVXYModelMapper>

#include <QSharedPointer>
#include <QDebug>
#include <QDir>

#include "uiwrapper.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication app(argc, argv);

    // QString filesDir { QCoreApplication::applicationDirPath() + "/SampleFiles/" };
    // QDir directory(filesDir);
    // // Получаем список файлов в директории
    // QStringList fileList { directory.entryList(QDir::Files) };


    // bool hasError {true};

    // /// Get measurement data
    // QSharedPointer<AbstractDataReader> dataReader(new SsdDataReader("SampleFiles/" + fileList[0]));
    // hasError = dataReader->readData();
    // if (hasError)
    // {
    //     qWarning() << "Can't read file. Stop";
    //     return -1;
    // }

    // const auto& measurements { dataReader->measurements() };
    // if (measurements.size() == 0)
    // {
    //     qWarning() << "Empty measurements";
    //     return -1;
    // }

    // const auto& firstElement { measurements.at(0) };
    // /// Set graph data
    // QSharedPointer<GraphModel> graphData(new GraphModel(firstElement));
    // graphData->addPoints(dataReader->measurements());
    // graphData->addInformation(dataReader->headers());
    // graphData->setFileNames(fileList);

    // QSharedPointer<QVXYModelMapper> seriesMapper(new QVXYModelMapper());
    // seriesMapper->setModel(graphData.data());
    // seriesMapper->setXColumn(0);
    // seriesMapper->setYColumn(1);

    QQmlApplicationEngine engine;
    UIWrapper uiWrapper;
    uiWrapper.init(engine.rootContext());
    uiWrapper.createGraph();

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
