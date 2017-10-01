#ifndef GAME_H
#define GAME_H
#include <QQuickItem>
#include <QSGNode>
#include <QSGFlatColorMaterial>
#include <QTimer>
#include <QKeyEvent>
#include <QObject>
#include <QSGGeometry>
#include <QQuickView>

#include "gui.h"
#include "models/playermodel.h"
#include "curver.h"
#include "itemfactory.h"
#include "wall.h"
#include "bot.h"
#include "network/server.h"
#include "network/client.h"

class Game : public QQuickItem
{
	Q_OBJECT
public:
	explicit Game(QQuickItem *parent = 0);
	~Game();

	Q_INVOKABLE void startGame();
	Q_INVOKABLE void processKey(Qt::Key key, bool release);
	Q_INVOKABLE void connectToHost(QString ip, int port);
	Q_INVOKABLE void sendChatMessage(QString msg);
	Q_INVOKABLE void serverReListen(quint16 port);
	Q_INVOKABLE void resetGame();

	QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *);
public slots:
signals:
	void postInfoBar(QString msg);
	void gameStarted();
private slots:
	void progress();
	void curverDied();
	void resetRound();
	void dimensionChanged();
	void connectedToServerChanged(bool connected);
	void tryStartGame();
private:
	std::vector<std::unique_ptr<Curver> > &getCurvers();

	QTimer gameTimer;
	QTime lastProgressTime;
	QTimer resetRoundTimer;
	QSGNode *rootNode;
	std::unique_ptr<ItemFactory> itemFactory;
	Wall wall;
	Server server;
	Client client;
	bool connectedToServer = false;
	bool started = false;
};

#endif // GAME_H
