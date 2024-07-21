#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>
#include <iostream>

#include "../include/logic.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setApplicationName("Classifier");
    app.setWindowIcon(QIcon(":/img/resources/world.png"));

    QQmlApplicationEngine engine;
    Logic logicModel;

    const QUrl url(QStringLiteral("qrc:/qml/components/Main.qml"));
    engine.rootContext()->setContextProperty("logic", &logicModel);
    engine.connect(&logicModel, &Logic::doClassification, [&logicModel]()
                   { std::cout << "Classify in internal classifier: " << std::endl; });
    engine.load(url);
    return app.exec();
}
