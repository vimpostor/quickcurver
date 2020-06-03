#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QQmlContext>

#include "models/playermodel.h"
#include "game.h"
#include "utility"
#include "models/itemmodel.h"
#include "settings.h"
#include "models/chatmodel.h"
#include "gamewatcher.h"
#include "mumble.h"

#ifdef QT_STATIC
#include <QQmlExtensionPlugin>
Q_IMPORT_PLUGIN(FluidCorePlugin)
Q_IMPORT_PLUGIN(FluidControlsPlugin)
Q_IMPORT_PLUGIN(FluidControlsPrivatePlugin)
Q_IMPORT_PLUGIN(FluidTemplatesPlugin)
#endif

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
	constexpr std::array<const char*, 4> customDpiVars = {"QT_DEVICE_PIXEL_RATIO", "QT_AUTO_SCREEN_SCALE_FACTOR", "QT_SCALE_FACTOR", "QT_SCREEN_SCALE_FACTORS"};
	if (Util::find_if(customDpiVars, [](auto dpiVar){ return qEnvironmentVariableIsSet(dpiVar); }) == customDpiVars.end()) {
		QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	}
	QQuickStyle::setStyle(QLatin1String("Material"));
#if defined(Q_OS_LINUX) || defined(Q_OS_WIN) || defined(Q_OS_MAC)
	const char* materialVariantName = "QT_QUICK_CONTROLS_MATERIAL_VARIANT";
	if (!qEnvironmentVariableIsSet(materialVariantName)) {
		// dense Material style, more suited for desktop targets
		qputenv(materialVariantName, "Dense");
	}
#endif
	QApplication app(argc, argv);

	qRegisterMetaType<Client::JoinStatus>("JoinStatus");

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

	// Mumble
#ifdef MUMBLE_SUPPORT
	Mumble::Api::get()->initialize();
#endif // MUMBLE_SUPPORT

	// register QML types here
	qmlRegisterType<Game>("Game", 1, 0, "Game");
	qmlRegisterType<Client>("Client", 1, 0, "Client");

	QQmlApplicationEngine engine;

	// context properties
	engine.rootContext()->setContextProperty("c_playerModel", &PlayerModel::getSingleton());
	engine.rootContext()->setContextProperty("c_itemModel", &ItemModel::getSingleton());
	engine.rootContext()->setContextProperty("c_chatModel", &ChatModel::getSingleton());
	engine.rootContext()->setContextProperty("c_settings", &Settings::getSingleton());

	engine.addImportPath(QCoreApplication::applicationDirPath() + QDir::separator() + QLatin1String("..") + QDir::separator() + QLatin1String("fluid") + QDir::separator() + QLatin1String("qml"));
	engine.addImportPath(QCoreApplication::applicationDirPath() + QDir::separator() + QLatin1String("qml"));
	engine.load(QUrl(QLatin1String("qrc:/main.qml")));
	if (engine.rootObjects().isEmpty()) {
		return -1;
	}
	return app.exec();
}
