#include "managerlogic.h"

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQuickView *view = new QQuickView;

    ManagerLogic logic(view);
    view->setSource(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
