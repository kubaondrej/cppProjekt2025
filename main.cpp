#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QCoreApplication>

#include "MapModel.h"
#include "UnitModel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // MAPA
    MapModel mapModel;
    mapModel.generate(10);

    // JEDNOTKY
    UnitModel unitModel;
    unitModel.addUnit(1, 1);
    unitModel.addUnit(3, 4);

    // REGISTRACE DO QML
    engine.rootContext()->setContextProperty("mapModel", &mapModel);
    engine.rootContext()->setContextProperty("unitModel", &unitModel);

    const QUrl url(QStringLiteral("qrc:/Main.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection
        );

    engine.load(url);
    return app.exec();
}
