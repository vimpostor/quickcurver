#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtGui>
#include <QtQuick>
#include "qcurver.h"
#include "game.h"
//#include "graphnode.h"
#include "segment.h"
#include <QGLFormat>

int main(int argc, char *argv[])
{
	//TODO: Fix Anti Aliasing!!!
	QGLFormat fmt;
	fmt = QGLFormat::defaultFormat();
	fmt.setSamples(16);
	QGLFormat::setDefaultFormat(fmt);

	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QGuiApplication app(argc, argv);

	qmlRegisterType<Game>("Game", 1, 0, "Game");
	QQmlApplicationEngine engine;
	engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    return app.exec();
}
