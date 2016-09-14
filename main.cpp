#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtGui>
#include <QtQuick>
#include "qcurver.h"
#include "game.h"
//#include "graphnode.h"
#include "segment.h"
#include "playerselector.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qmlRegisterType<Game>("Game", 1, 0, "Game");


    QQuickView view;
    QSurfaceFormat format = view.format();
    format.setSamples(16);
    view.setFormat(format);
    view.setSource(QUrl("qrc:///main.qml"));

	playerSelector playerSelectorHandler(view.rootObject()->findChild<Game*>("game"));
	QObject::connect(view.rootObject(), SIGNAL(addnewPlayer()), &playerSelectorHandler, SLOT(newPlayerSlot()));
	QObject::connect(view.rootObject(), SIGNAL(colorChanged(int,QColor)), &playerSelectorHandler, SLOT(colorChanged(int,QColor)));

    view.show();

    return app.exec();
}
