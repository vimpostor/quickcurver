#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtGui>
#include <QtQuick>
#include "qcurver.h"
#include "game.h"
#include "segment.h"
#include <QGLFormat>

int main(int argc, char *argv[])
{
	//TODO: Fix Anti Aliasing!!!
    if (!qputenv("QSG_RENDER_LOOP", "basic")) {  //threaded render_loop, which is default on non-mesa drivers, breaks drawing
        qDebug() << "Failed to set QSG_RENDER_LOOP=basic";
    }
    QGLFormat fmt;
	fmt = QGLFormat::defaultFormat();
	fmt.setSamples(16);
	QGLFormat::setDefaultFormat(fmt);

	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QGuiApplication app(argc, argv);

	qmlRegisterType<Game>("Game", 1, 0, "Game");
	QQmlApplicationEngine engine;
	QQmlComponent component(&engine, QUrl(QLatin1String("qrc:/main.qml")));
	QObject *object = component.create();
	Game* game = object->findChild<Game*>("game");
	game->setQmlObject(object);

    return app.exec();
}
