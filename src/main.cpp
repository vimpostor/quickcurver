#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

#include "game.hpp"
#include "gamewatcher.hpp"
#include "models/chatmodel.hpp"
#include "models/itemmodel.hpp"
#include "models/playermodel.hpp"
#include "mumble.hpp"
#include "settings.hpp"
#include "utility"
#include "version.hpp"

/**
 * @mainpage Introduction
 *
 * This is the documentation for the Quickcurver project, a free and open-source implementation of Achtung die Kurve written with Qt.
 *
 * A good starting point for reading this documentation is the Game class.
 */


int main(int argc, char *argv[]) {
	// threaded render_loop, which is default on non-mesa drivers, breaks drawing
	qputenv("QSG_RENDER_LOOP", "basic");
	QGuiApplication app(argc, argv);

	qRegisterMetaType<Client::JoinStatus>("JoinStatus");

	QCoreApplication::setApplicationName("Quickcurver");
	QCoreApplication::setApplicationVersion(Version::version_string());

	QCommandLineParser parser;
	parser.setApplicationDescription("Quickcurver");
	parser.addHelpOption();
	parser.addVersionOption();
	parser.process(app);

	// headless server
	if (Settings::getSingleton().getOffscreen()) {
		GameWatcher gameWatcher;
		gameWatcher.start();
		return app.exec();
	}

	QFontDatabase::addApplicationFont("/MaterialSymbols.woff2");

	// Mumble
#ifdef MUMBLE_SUPPORT
	Mumble::Api::get()->initialize();
#endif // MUMBLE_SUPPORT

	// register QML types here
	qmlRegisterType<Game>("Game", 1, 0, "Game");
	qmlRegisterType<Client>("Client", 1, 0, "Client");

	QQmlApplicationEngine engine;
	engine.addImportPath(QStringLiteral(":/"));

	// context properties
	engine.rootContext()->setContextProperty("c_playerModel", &PlayerModel::getSingleton());
	engine.rootContext()->setContextProperty("c_itemModel", &ItemModel::getSingleton());
	engine.rootContext()->setContextProperty("c_chatModel", &ChatModel::getSingleton());
	engine.rootContext()->setContextProperty("c_settings", &Settings::getSingleton());

	engine.loadFromModule("Backend", "Main");
	if (engine.rootObjects().isEmpty()) {
		return -1;
	}
	return app.exec();
}
