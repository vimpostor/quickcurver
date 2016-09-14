#include "game.h"

Game::Game(QQuickItem *parent) : QQuickItem(parent) {
	segment::initRand();
	for (int i = 0; i < MAXPLAYERCOUNT; i++) {
		controls[i][0] = Qt::Key_Left;
		controls[i][1] = Qt::Key_Right;
	}
}

Game::~Game() {
	timer->deleteLater();
}

QSGNode *Game::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) {
	//we don't need oldNode
	return node;
}

void Game::start() {
	timer = new QTimer(this);
	nextRoundTimer = new QTimer(this);
	node = new QSGNode;
	for (int i = 0; i < playercount; i++) {
		alive[i] = true;
		score[i] = 0;
		curver[i] = new QCurver(colors[i], node);
		connect(curver[i], SIGNAL(died(QCurver*)), this, SLOT(curverDied(QCurver*)));
		connect(curver[i], SIGNAL(requestIntersectionChecking(QPointF,QPointF)), this, SLOT(checkforIntersection(QPointF,QPointF)));
	}
	setFlag(ItemHasContents, true);

	connect(timer, SIGNAL(timeout()), this, SLOT(progress()));
	lastTime = QTime::currentTime();
	lastItemSpawn = lastTime;
	nextItemSpawn = segment::randInt(5000,10000);
	timer->start(30);
}

void Game::changeColor(int index, QColor color) {
	colors[index] = color;
}

void Game::progress() {
	//calculate time since last progress()
	float deltat = (float) lastTime.msecsTo(QTime::currentTime())/1000;
	lastTime = QTime::currentTime();
	if (lastItemSpawn.msecsTo(lastTime) > nextItemSpawn) {
		qDebug() << "Item spawned";
		int i;
		for (i = 0; items[i] != NULL; i++) { //find first free item slot
		}
		items[i] = new FastItem(node);
		lastItemSpawn = lastTime;
		nextItemSpawn = segment::randInt(1000,10000);
	}
	for (int i = 0; i < playercount; i++) {
		if (alive[i]) {
			//check for item collision
			for (int j = 0; j < MAXITEMCOUNT; j++) {
				if (items[j] != NULL && items[j]->testCollision(curver[i]->getPos())) {
					//use item
					items[j]->useItem(playercount, curver, curver[i]);
					items[j] = NULL; //dont worry it will delete it by its own
				}
			}
			curver[i]->progress(deltat);
		}
	}
	update();
}

Q_INVOKABLE void Game::sendKey(Qt::Key k) {
	for (int i = 0; i < playercount; i++) {
		if (controls[i][0] == k) {
			curver[i]->rotating = ROTATE_LEFT;
		} else if (controls[i][1] == k) {
			curver[i]->rotating = ROTATE_RIGHT;
		}
	}
}

Q_INVOKABLE void Game::releaseKey(Qt::Key k) {
	for (int i = 0; i < playercount; i++) {
		if (controls[i][0] == k || controls[i][1] == k) {
			curver[i]->rotating = ROTATE_NONE;
		}
	}
}

void Game::addPlayer() {
	playercount++;
	qDebug() << "added player " << playercount;
}

void Game::curverDied(QCurver *who) {
	int i;
	for (i = 0; curver[i] != who; i++) {
		if (alive[i]) //if he is still alive, increase his score
			score[i]++;
	}
	alive[i] = false;
	qDebug() << "Player " << i << " died";
	i++;
	for (i = 0; i < playercount; i++) {
		if (alive[i])
			score[i]++;
	}
	int stillAlive = 0;
	int alivePlayer;
	for (i = 0; i < playercount; i++) {
		if (alive[i]) {
			stillAlive++;
			alivePlayer = i;
		}
	}
	if (stillAlive == 1) {
		qDebug() << "Player " << alivePlayer << " has won!";
		nextRound();
	}
}

void Game::checkforIntersection(QPointF a, QPointF b) {
	QCurver* who = (QCurver*) QObject::sender();
	bool c = false;
	for (int i = 0; i < playercount && !c; i++) {
		c = curver[i]->checkforIntersection(a, b);
	}
	if (c) {
		emit who->died(who);
	}
}

void Game::changeControls(int index, Qt::Key k, bool isRight) {
	controls[index][isRight? 1 : 0] = k;
	qDebug() << "Assigned key to player "<< index <<": " + QKeySequence(k).toString();
}

void Game::nextRound() {
	nextRoundTimer->singleShot(2000, this, SLOT(startNextRound()));
}

void Game::startNextRound() {
	timer->stop();
	roundCount++;
	for (int i = 0; i < playercount; i++) {
		curver[i]->reset();
		alive[i] = true;
	}
	for (int i = 0; i < MAXITEMCOUNT; i++) {
		if (items[i] != NULL) {
//			connect(this, SIGNAL(stopItemTimer()), items[i], SLOT(stopTimer()));
//			emit stopItemTimer();
//			disconnect(this, SIGNAL(stopItemTimer()), items[i], SLOT(stopTimer()));
			delete items[i];
		}
		items[i] = NULL;
	}
	lastItemSpawn = QTime::currentTime();
	timer->start();
}
