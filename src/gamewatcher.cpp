#include "gamewatcher.hpp"

/**
 * @brief Constructs a GameWatcher object and connects all signals of the commandline reader with slots related to the Game.
 * @param parent The parent object
 */
GameWatcher::GameWatcher(QObject *parent)
	: QObject(parent) {
	connect(&cliReader, &CommandlineReader::addBot, PlayerModel::get(), &PlayerModel::appendBot);
	connect(&cliReader, &CommandlineReader::chat, &game, &Game::sendChatMessage);
	connect(&cliReader, &CommandlineReader::itemSpawn, ItemModel::get(), &ItemModel::setProbability);
	connect(&cliReader, &CommandlineReader::itemWait, [](int min, int max) { Settings::get()->setItemSpawnIntervalMin(min); Settings::get()->setItemSpawnIntervalMax(max); });
	connect(&cliReader, &CommandlineReader::listen, &game, &Game::serverReListen);
	connect(&cliReader, &CommandlineReader::logicUpdate, Settings::get(), &Settings::setUpdatesPerSecond);
	connect(&cliReader, &CommandlineReader::networkUpdate, Settings::get(), &Settings::setNetworkCurverBlock);
	connect(&cliReader, &CommandlineReader::quit, this, &GameWatcher::quit, Qt::QueuedConnection);
	// TODO: Remove player must call the slot in Server
	connect(&cliReader, &CommandlineReader::remove, PlayerModel::get(), &PlayerModel::removePlayer);
	connect(&cliReader, &CommandlineReader::removeBots, PlayerModel::get(), &PlayerModel::removeBots);
	connect(&cliReader, &CommandlineReader::reset, &game, &Game::resetGame);
	connect(&cliReader, &CommandlineReader::resize, Settings::get(), &Settings::setDimension);
	connect(&cliReader, &CommandlineReader::start, &game, &Game::startGame);
	connect(&cliReader, &CommandlineReader::targetScore, Settings::get(), &Settings::setTargetScore);

	// copy ingame chat to terminal
	connect(ChatModel::get(), &ChatModel::newMessage, this, &GameWatcher::printChatMessage);
}

/**
 * @brief Starts the GameWatcher by parsing the commandline input.
 */
void GameWatcher::start() {
	cliReader.runAsync();
}

/**
 * @brief Quits the whole operation and the program.
 */
void GameWatcher::quit() {
	QCoreApplication::quit();
}

/**
 * @brief Prints a chat message
 * @param username The author of the chat message
 * @param message The message content
 */
void GameWatcher::printChatMessage(QString username, QString message) {
	qInfo() << username << message;
}
