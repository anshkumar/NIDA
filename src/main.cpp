#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "imageanalysis.h"
#include <qjsonrpc/qjsonrpchttpclient.h>


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    ImageAnalysis img;
    engine.rootContext()->setContextProperty("img", &img);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
