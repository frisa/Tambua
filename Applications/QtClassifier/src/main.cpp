#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>
#include <iostream>

#include "../include/logic.h"
#include "classifier.h"

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
                   {
                       std::cout << "Classify in internal classifier: " << std::endl;
                       std::string result = DoClassification(logicModel.getParameter1().toStdString(),
                                        logicModel.getParameter2().toStdString(),
                                        logicModel.getParameter3().toStdString());
                       logicModel.setParameter4(QString::fromStdString(result)); });
    engine.load(url);
    return app.exec();
}
