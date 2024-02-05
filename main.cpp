#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "uiwrapper.h"
#include "graphpainter.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    UIWrapper uiWrapper;
    uiWrapper.init(engine.rootContext());
    // uiWrapper.createGraph();

    qmlRegisterType<GraphPainter>("CustomGraph", 1, 0, "GraphPainter");

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
