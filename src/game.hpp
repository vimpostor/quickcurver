#pragma once

#include <QKeyEvent>
#include <QObject>
#include <QQuickItem>
#include <QQuickView>
#include <QSGFlatColorMaterial>
#include <QSGGeometry>
#include <QSGNode>
#include <QTimer>

#include "bot.hpp"
#include "curver.hpp"
#include "gui.hpp"
#include "itemfactory.hpp"
#include "models/playermodel.hpp"
#include "network/client.hpp"
#include "network/server.hpp"
#include "wall.hpp"

/**
 * @brief A class representing an entire game
 *
 * This class handles a game and manages all the things involved with it such as resetting rounds and updating the game each frame.
 */
class Game : public QQuickItem {
	Q_OBJECT

	Q_PROPERTY(Client *client READ getClient() CONSTANT)
	Q_PROPERTY(bool isStarted MEMBER started NOTIFY gameStarted)
public:
	explicit Game(QQuickItem *parent = 0);
	~Game();

	Q_INVOKABLE void startGame();
	Q_INVOKABLE void processKey(Qt::Key key, bool release);
	Q_INVOKABLE void connectToHost(QString ip, int port);
	Q_INVOKABLE void sendChatMessage(QString msg);
	Q_INVOKABLE void serverReListen(quint16 port);
	Q_INVOKABLE void resetGame();
	Q_INVOKABLE Client *getClient();

	QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *);
public slots:
signals:
	/**
	 * @brief Emitted, when something wants to post the GUI infobar
	 * @param msg The message to display
	 */
	void postInfoBar(QString msg);
	/**
	 * @brief Emitted, when the game started
	 */
	void gameStarted();
private slots:
	void progress();
	void curverDied();
	void resetRound();
	void tryStartGame();
private:
	std::vector<std::unique_ptr<Curver>> &getCurvers();

	/**
	 * @brief The timer responsible for the game main loop
	 */
	QTimer gameTimer;
	/**
	 * @brief The time of the last game logic update
	 */
	QTime lastProgressTime;
	/**
	 * @brief The timer responsible for resetting the round after all players died
	 */
	QTimer resetRoundTimer;
	/**
	 * @brief The root node in the scene graph
	 */
	QSGNode *rootNode;
	/**
	 * @brief The item factory of this game
	 */
	std::unique_ptr<ItemFactory> itemFactory;
	/**
	 * @brief A node representing the wall
	 */
	Wall wall;
	/**
	 * @brief The server instance
	 */
	Server server;
	/**
	 * @brief The client instance
	 */
	Client client;
	/**
	 * @brief Whether the game already started
	 */
	bool started = false;
	/**
	 * @brief Whether the winner was already announced
	 */
	bool winnerAnnounced = false;
};
