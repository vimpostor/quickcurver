#include "gamewatcher.h"

/**
 * @brief Constructs a GameWatcher object and connects all signals of the commandline reader with slots related to the Game.
 * @param parent The parent object
 */
GameWatcher::GameWatcher(QObject *parent) : QObject(parent)
{
	connect(&cliReader, &CommandlineReader::addBot, &PlayerModel::getSingleton(), &PlayerModel::appendBot);
	connect(&cliReader, &CommandlineReader::itemSpawn, &ItemModel::getSingleton(), &ItemModel::setProbability);
	connect(&cliReader, &CommandlineReader::itemWait, [](int min, int max){ Settings::getSingleton().setItemSpawnIntervalMin(min); Settings::getSingleton().setItemSpawnIntervalMax(max); });
	connect(&cliReader, &CommandlineReader::listen, &game, &Game::serverReListen);
	connect(&cliReader, &CommandlineReader::logicUpdate, &Settings::getSingleton(), &Settings::setUpdatesPerSecond);
	connect(&cliReader, &CommandlineReader::networkUpdate, &Settings::getSingleton(), &Settings::setNetworkCurverBlock);
	connect(&cliReader, &CommandlineReader::quit, this, &GameWatcher::quit, Qt::QueuedConnection);
	connect(&cliReader, &CommandlineReader::reset, &game, &Game::resetGame);
	connect(&cliReader, &CommandlineReader::start, &game, &Game::startGame);
	connect(&cliReader, &CommandlineReader::targetScore, &Settings::getSingleton(), &Settings::setTargetScore);
}

/**
 * @brief Starts the GameWatcher by parsing the commandline input.
 */
void GameWatcher::start()
{
	cliReader.runAsync();
}

/**
 * @brief Quits the whole operation and the program.
 */
void GameWatcher::quit()
{
	QCoreApplication::quit();
}
