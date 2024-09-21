#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QQmlContext>
#include "programm.h"

int main(int argc, char *argv[])
{
    //DH DHA{};
    //DH::Pos XYZ = DHA.CalcLastJointPos({ 15.f, -50.f, -60.f, 95.f, 50.f, 0.f });
    QGuiApplication App(argc, argv);
    QQmlApplicationEngine Engine;
    QQuickStyle::setStyle("Material");
    const QUrl Url(u"qrc:/dha_qml/main.qml"_qs);
    QObject::connect(&Engine, &QQmlApplicationEngine::objectCreated, &App,
                     [Url](QObject *obj, const QUrl &objUrl) {
                         if (!obj && Url == objUrl)
                             QCoreApplication::exit(-1);
                     }, Qt::QueuedConnection);
    Programm Prog;
    Engine.rootContext()->setContextProperty("backend", Prog.GetBackend());
    Engine.load(Url);
    return App.exec();
}
