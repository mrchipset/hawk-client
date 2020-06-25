#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFile>
#include <QQmlContext>
#include "Core/JsonParser.h"
#include "Core/PhonixClient.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    PhonixClient client("http://phonix.mrchip.info/");
    engine.rootContext()->setContextProperty("hawkClient", &client);
    engine.addImportPath(QStringLiteral("qrc:/"));
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
    return app.exec();
}
