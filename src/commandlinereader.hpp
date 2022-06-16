#ifndef COMMANDLINEREADER_H
#define COMMANDLINEREADER_H

#include <QDebug>
#include <QObject>
#include <QTextStream>
#include <QtConcurrent/QtConcurrent>

/**
 * @brief This class is able to read from the command line in an asynchronous way
 */
class CommandlineReader : public QObject {
	Q_OBJECT
public:
	explicit CommandlineReader(QObject *parent = nullptr);
	void runAsync();
	void run();

signals:
	/**
	 * @brief The user wants to add a bot
	 */
	void addBot();
	/**
	 * @brief The user wants to broadcast a chat message
	 * @param message The message to broadcast
	 */
	void chat(QString message);
	/**
	 * @brief The user wants to set item spawn probabilities
	 * @param index The index of the item
	 * @param prob The probability of the item to spawn
	 */
	void itemSpawn(int index, float prob);
	/**
	 * @brief The user wants to set the delay between item spawns
	 * @param min The minimum time to wait in milliseconds
	 * @param max The maximum time to wait in milliseconds
	 */
	void itemWait(int min, int max);
	/**
	 * @brief The user wants the server to listen on a new port
	 * @param port The port to listen on
	 */
	void listen(quint16 port);
	/**
	 * @brief The user wants to change the logic update rate
	 * @param rate The new rate in updates/second
	 *
	 * Note: This must be set, before the game is started.
	 */
	void logicUpdate(int rate);
	/**
	 * @brief The user wants to change the network update rate
	 * @param rate The new update rate
	 */
	void networkUpdate(int rate);
	/**
	 * @brief The user wants to quit the program
	 */
	void quit();
	/**
	 * @brief Remove a player from the game
	 * @param index The player to remove
	 */
	void remove(int index);
	/**
	 * @brief Remove all bots
	 */
	void removeBots();
	/**
	 * @brief The user wants to reset the game
	 */
	void reset();
	/**
	 * @brief The user wants to resize the game
	 * @param dimension The new dimension
	 */
	void resize(QPoint dimension);
	/**
	 * @brief The user wants to start the game
	 */
	void start();
	/**
	 * @brief The user wants to change the target score
	 * @param targetScore The score that once reached determines the winner
	 */
	void targetScore(int targetScore);
private:
	bool checkList(const std::list<QString> &l);
	bool takeFloat(float &result, std::list<QString> &l, QString info);
	bool takeInt(int &result, std::list<QString> &l, QString info);
	bool takeString(QString &result, std::list<QString> &l, QString info);
};

#endif // COMMANDLINEREADER_H
