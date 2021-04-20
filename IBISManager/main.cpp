#include "managerlogic.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    ManagerLogic logic(&engine);
    engine.load("qrc:/main.qml");
    return app.exec();
}

//int main(int argc, char *argv[])
//{
//    QApplication app(argc, argv);
//
//    QQuickView *view = new QQuickView;
//
//    ManagerLogic logic(view);
//    view->setSource(QUrl(QStringLiteral("qrc:/main.qml")));
//
//    return app.exec();
//}
