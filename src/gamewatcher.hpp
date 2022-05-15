#ifndef GAMEWATCHER_H
#define GAMEWATCHER_H

#include <QObject>

#include "commandlinereader.hpp"
#include "game.hpp"

/**
 * @brief Watches a CommandlineReader to interact with the game. This is the CLI implementation of the game.
 */
class GameWatcher : public QObject
{
	Q_OBJECT
public:
	explicit GameWatcher(QObject *parent = nullptr);
	void start();
private slots:
	void quit();
	void printChatMessage(QString username, QString message);
private:
	/**
	 * @brief The commandline reader interface
	 */
	CommandlineReader cliReader;
	/**
	 * @brief The Game object
	 */
	Game game;
};

#endif // GAMEWATCHER_H
