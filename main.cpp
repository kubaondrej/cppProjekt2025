#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QCoreApplication>

#include "mapmodel.h"
#include "unitmodel.h"
#include "gamecontroller.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    MapModel mapModel;

    UnitModel unitModel;

    GameController gameController(&unitModel, &mapModel);

    engine.rootContext()->setContextProperty("mapModel", &mapModel);
    engine.rootContext()->setContextProperty("unitModel", &unitModel);
    engine.rootContext()->setContextProperty("gameController", &gameController);


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
