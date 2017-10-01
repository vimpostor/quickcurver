#include "game.h"

Game::Game(QQuickItem *parent) : QQuickItem(parent)
{
	/* Create a new root node. No, this does not leak memory, the Qt scene graph automatically deletes this node at the end.
	 * The scene graph knows this root node from updatePaintNode and will destroy it, when the window closes.
	 * Do NOT under any circumstances delete this node, or otherwise Qt will try to double free it,
	 * which will result in a big Qt backtrace when the window closes.
	 * Trust me, you do not want to debug this again.
	*/
	rootNode = new QSGNode();
	itemFactory = std::make_unique<ItemFactory>(rootNode);
	// tell the playermodel, what the root node is, so that it can tell its curvers
	PlayerModel::getSingleton().setRootNode(this->rootNode);
	connect(&PlayerModel::getSingleton(), SIGNAL(curverDied()), this, SLOT(curverDied()));
	connect(&PlayerModel::getSingleton(), SIGNAL(playerModelChanged()), &server, SLOT(broadcastPlayerModel()));
	connect(&ItemModel::getSingleton(), SIGNAL(itemSpawned(bool,uint,int,QPointF,Item::AllowedUsers,int)), &server, SLOT(broadcastItemData(bool,uint,int,QPointF,Item::AllowedUsers,int)));
	wall.setParentNode(rootNode);
	connect(&client, SIGNAL(integrateItem(bool,uint,int,QPointF,Item::AllowedUsers,int)), itemFactory.get(), SLOT(integrateItem(bool,uint,int,QPointF,Item::AllowedUsers,int)));
	connect(&client, SIGNAL(resetRound()), this, SLOT(resetRound()));
	connect(&Settings::getSingleton(), SIGNAL(dimensionChanged()), this, SLOT(dimensionChanged()));
	// GUI signals
	connect(&Gui::getSingleton(), SIGNAL(postInfoBar(QString)), this, SIGNAL(postInfoBar(QString)));
	connect(&Gui::getSingleton(), SIGNAL(startGame()), this, SLOT(tryStartGame()));

	// client signals
	connect(&client, SIGNAL(connectedToServerChanged(bool)), this, SLOT(connectedToServerChanged(bool)));

	connect(&gameTimer, SIGNAL(timeout()), this, SLOT(progress()));
	// tell QtQuick, that this component wants to draw stuff
	setFlag(ItemHasContents);
}

Game::~Game() {
	// we do manual memory management with every child node, so we have to remove every child node to prevent a double free
	rootNode->removeAllChildNodes();
}

void Game::startGame()
{
	tryStartGame();
	lastProgressTime = QTime::currentTime();
	Util::for_each(getCurvers(), [](const std::unique_ptr<Curver> &c){ c->start(); });
	itemFactory->resetRound();
	// 60 FPS = 16 ms interval
	gameTimer.start(16);
}

void Game::processKey(Qt::Key key, bool release)
{
	Util::for_each(getCurvers(), [&](std::unique_ptr<Curver> &c){ c->processKey(key, release); });
	if (connectedToServer) {
		client.processKey(key, release);
	}
}

void Game::connectToHost(QString ip, int port)
{
	QHostAddress addr = QHostAddress(ip);
	client.connectToHost(addr, port);
}

void Game::sendChatMessage(QString msg)
{
	if (connectedToServer) {
		client.sendChatMessage(msg);
	} else {
		server.broadcastChatMessage(msg);
	}
}

void Game::serverReListen(quint16 port)
{
	server.reListen(port);
}

void Game::resetGame()
{
	resetRound();
	Util::for_each(getCurvers(), [](const auto &c){ c->totalScore = 0; });
}

QSGNode *Game::updatePaintNode(QSGNode *, QQuickItem::UpdatePaintNodeData *)
{
	return rootNode;
}

void Game::progress()
{
	QTime currentTime = QTime::currentTime();
	int deltat = lastProgressTime.msecsTo(currentTime);
	lastProgressTime = currentTime;
	for (auto &c : getCurvers()) {
		if (c->isAlive()) {
			if (c->controller == Curver::CONTROLLER_BOT) {
				Bot::makeMove(c.get());
			}
			c->progress(deltat, getCurvers());
			c->checkForWall();
		}
	}
	itemFactory->update();

	update();
	server.broadcastCurverData();
}

void Game::curverDied()
{
	Util::for_each(getCurvers(), [](const auto &c){ if (c->isAlive()) c->increaseScore(); });
	auto maxScorer = Util::max_element(getCurvers());
	if ((*maxScorer)->totalScore >= Settings::getSingleton().getTargetScore()) {
		// we have a winner
		server.broadcastChatMessage((*maxScorer)->userName + " won!");
	}
	// check if only one player is remaining
	if (Util::count_if(getCurvers(), [](const auto &c){ return c->isAlive(); }) == 1) {
		resetRoundTimer.singleShot(Settings::getSingleton().getRoundTimeOut(), this, SLOT(resetRound()));
	}
}

void Game::resetRound()
{
	itemFactory->resetRound();
	Util::for_each(getCurvers(), [](const auto &c){ c->resetRound(); });
	server.resetRound();
}

void Game::dimensionChanged()
{
	wall.updateDimension();
}

void Game::connectedToServerChanged(bool connected)
{
	this->connectedToServer = connected;
}

void Game::tryStartGame()
{
	if (!started) {
		started = true;
		emit gameStarted();
	}
}

std::vector<std::unique_ptr<Curver> > &Game::getCurvers()
{
	return PlayerModel::getSingleton().getCurvers();
}
