#include "game.hpp"

/**
 * @brief Constructs a Game with the given parent.
 *
 * \a parent is used to draw everything onto the GUI window.
 * @param parent The parent to draw on
 */
Game::Game(QQuickItem *parent)
	: QQuickItem(parent) {
	/* Create a new root node. No, this does not leak memory, the Qt scene graph automatically deletes this node at the end.
	 * The scene graph knows this root node from updatePaintNode and will destroy it, when the window closes.
	 * Do NOT under any circumstances delete this node, or otherwise Qt will try to double free it,
	 * which will result in a big Qt backtrace when the window closes.
	 * Trust me, you do not want to debug this again.
	*/
	rootNode = new QSGNode();
	itemFactory = std::make_unique<ItemFactory>(rootNode);
	// notify the item factory about window changes
	connect(this, &QQuickItem::windowChanged, itemFactory.get(), &ItemFactory::setWindow);
	// tell the playermodel, what the root node is, so that it can tell its curvers
	PlayerModel::get()->setRootNode(this->rootNode);
	connect(PlayerModel::get(), &PlayerModel::curverDied, this, &Game::curverDied);
	connect(PlayerModel::get(), &PlayerModel::playerModelChanged, &server, &Server::broadcastPlayerModel);
	connect(ItemModel::get(), &ItemModel::itemSpawned, &server, &Server::broadcastItemData);
	wall.setParentNode(rootNode);
	connect(&client, &Client::integrateItem, itemFactory.get(), &ItemFactory::integrateItem);
	connect(&client, &Client::resetRound, this, &Game::triggerResetRound);
	connect(&client, &Client::updateGraphics, this, &QQuickItem::update);
	// GUI signals
	connect(&Gui::getSingleton(), &Gui::postInfoBar, this, &Game::postInfoBar);
	connect(&Gui::getSingleton(), &Gui::startGame, this, &Game::tryStartGame);

	connect(&gameTimer, &QTimer::timeout, this, &Game::progress);
	// tell QtQuick, that this component wants to draw stuff
	setFlag(ItemHasContents);
}

Game::~Game() {
	// we do manual memory management with every child node, so we have to remove every child node to prevent a double free
	rootNode->removeAllChildNodes();
}

/**
 * @brief Starts the game
 */
void Game::startGame() {
	tryStartGame();
	lastProgressTime = QTime::currentTime();
	std::ranges::for_each(getCurvers(), [](const std::unique_ptr<Curver> &c) { c->start(); });
	itemFactory->resetRound();
	// 60 FPS = 16 ms interval
	gameTimer.start(static_cast<int>(1000.f / Settings::get()->getUpdatesPerSecond()));
}

/**
 * @brief Processes a key
 *
 * Checks if this key was registered to trigger any rotation changes.
 * @param key The key to process
 * @param release Whether the key was pressed or released
 */
void Game::processKey(Qt::Key key, bool release) {
	std::ranges::for_each(getCurvers(), [&](auto &c) { c->processKey(key, release); });
	if (getClient()->getJoinStatus() == Client::JoinStatus::JOINED) {
		client.processKey(key, release);
	}
}

/**
 * @brief Connects as a client to the given host
 * @param ip The IP address of the host
 * @param port The port that the host is listening on
 */
void Game::connectToHost(QString ip, int port) {
	client.connectToHost(ip, port);
}

/**
 * @brief Sends a chat message
 *
 * If this instance is the Server of a game, the chat message will be broadcasted to all clients.
 * @param msg The message to send
 */
void Game::sendChatMessage(QString msg) {
	if (getClient()->getJoinStatus() == Client::JoinStatus::JOINED) {
		client.sendChatMessage(msg);
	} else {
		server.broadcastChatMessage(msg);
	}
}

/**
 * @brief Reconfigures the server to listen on a different port
 * @param port The port to listen on
 */
void Game::serverReListen(quint16 port) {
	server.reListen(port);
}

/**
 * @brief Resets the entire game
 */
void Game::resetGame() {
	triggerResetRound();
	std::ranges::for_each(getCurvers(), [](const auto &c) { c->totalScore = 0; });
	winnerAnnounced = false;
	PlayerModel::get()->forceRefresh();
}

/**
 * @brief Returns the Client belonging to this Game
 * @return The Client
 */
Client *Game::getClient() {
	return &client;
}

/**
 * @brief Called by the scene graph. This is called before the screen is redrawn.
 * @return Always return Game::rootNode
 */
QSGNode *Game::updatePaintNode(QSGNode *, QQuickItem::UpdatePaintNodeData *) {
	// check if round should be reset
	if (triggerReset) {
		resetRound();
	}

	int deltat = Util::getTimeDiff(lastProgressTime);
	lastProgressTime = QTime::currentTime();
	for (auto &c : getCurvers()) {
		if (c->isAlive()) {
			if (c->controller == Curver::Controller::CONTROLLER_BOT) {
				Bot::makeMove(*c.get());
			}
		}
		c->progress(deltat, getCurvers());
	}
	itemFactory->update();
	rootNode->markDirty(QSGNode::DirtyStateBit::DirtyGeometry);

	return rootNode;
}

/**
 * @brief Updates the game's logic respecting how much time actually passed by
 */
void Game::progress() {
	update();
	server.broadcastCurverData();
}

/**
 * @brief Called, when a curver died
 *
 * This method taskes care of the score board and checks if a new round is due.
 */
void Game::curverDied() {
	std::ranges::for_each(getCurvers(), [](const auto &c) { if (c->isAlive()) c->increaseScore(); });
	auto maxScorer = std::ranges::max_element(getCurvers());
	if ((*maxScorer)->totalScore >= Settings::get()->getTargetScore() && !winnerAnnounced) {
		// we have a winner
		winnerAnnounced = true;
		server.broadcastChatMessage((*maxScorer)->userName + " won!");
	}
	// check if only one player is remaining
	if (!resetPending && std::ranges::count_if(getCurvers(), [](const auto &c) { return c->isAlive(); }) < 2) {
		resetPending = true;
		resetRoundTimer.singleShot(Settings::get()->getRoundTimeOut(), this, &Game::triggerResetRound);
	}
}

/**
 * @brief Resets the round
 */
void Game::resetRound() {
	itemFactory->resetRound();
	std::ranges::for_each(getCurvers(), [](const auto &c) { c->resetRound(); });
	server.resetRound();
	resetPending = false;
	triggerReset = false;
}

/**
 * @brief Sets a flag so that the next update event iteration will reset the round
 *
 * We cannot just immediately reset, because a round reset will touch some nodes, which must only happen inside the render thread.
 */
void Game::triggerResetRound() {
	this->triggerReset = true;
}

/**
 * @brief Tries to start the game.
 *
 * This method does nothing, if the game already started.
 */
void Game::tryStartGame() {
	if (!started) {
		started = true;
		gameStarted();
	}
}

/**
 * @brief A helper method to get a vector of all curvers
 * @return A vector containing all curvers
 */
std::vector<std::unique_ptr<Curver>> &Game::getCurvers() {
	return PlayerModel::get()->getCurvers();
}
