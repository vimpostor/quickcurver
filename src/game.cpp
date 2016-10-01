#include "game.h"

Game::Game(QQuickItem *parent) : QQuickItem(parent) {
	segment::initRand();
	for (int i = 0; i < MAXPLAYERCOUNT; i++) {
		controls[i][0] = Qt::Key_Left;
		controls[i][1] = Qt::Key_Right;
		controlledByAI[i] = false;
	}
}

Game::~Game() {
//	timer->deleteLater();
}

QSGNode *Game::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) {
	(void) oldNode; //suppresses unused variable warning
	return node;
}

void Game::start() {
	timer = new QTimer(this);
	nextRoundTimer = new QTimer(this);
	node = new QSGNode;
	wall = new wallNode(node);
	for (int i = 0; i < playercount; i++) {
		alive[i] = true;
		score[i] = 0;
		roundScore[i] = 0;
		curver[i] = new QCurver(node, colors[i], baseSpeed);
		if (controlledByAI[i]) {
			ai[i] = new AIController(curver[i], curver, playercount);
		}
		connect(curver[i], SIGNAL(died(QCurver*)), this, SLOT(curverDied(QCurver*)));
		connect(curver[i], SIGNAL(requestIntersectionChecking(QPointF,QPointF)), this, SLOT(checkforIntersection(QPointF,QPointF)));
	}
	for (int i = 0; i < MAXITEMCOUNT; i++) {
		items[i] = NULL;
	}
	setFlag(ItemHasContents, true);

	connect(timer, SIGNAL(timeout()), this, SLOT(progress()));
	lastTime = QTime::currentTime();
	lastItemSpawn = lastTime;
	nextItemSpawn = segment::randInt(5000,10000);
	timer->start(timerInterval);
}

void Game::setColor(int index, QColor color) {
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
		items[i]->setRound(roundCount);
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
			//let the AI make its move now
			if (controlledByAI[i] && ((frameCount+i) % AIINTERVAL == 0)) {
				qDebug() << "Player " << i << "is making a move";
				ai[i]->makeMove(deltat);
				qDebug() << "and he finished";
			}
			curver[i]->progress(deltat);
		}
	}
	frameCount++;
	update();
}

void Game::sendKey(Qt::Key k) {
	for (int i = 0; i < playercount; i++) {
		if (!controlledByAI[i]) {
			if (controls[i][0] == k) {
				curver[i]->rotating = ROTATE_LEFT;
			} else if (controls[i][1] == k) {
				curver[i]->rotating = ROTATE_RIGHT;
			}
		}
	}
}

void Game::releaseKey(Qt::Key k) {
	for (int i = 0; i < playercount; i++) {
		if (!controlledByAI[i]) {
			if (controls[i][0] == k || controls[i][1] == k) {
				curver[i]->rotating = ROTATE_NONE;
			}
		}
	}
}

void Game::addPlayer() {
	playercount++;
	// qDebug() << "added player " << playercount;
}

void Game::curverDied(QCurver *who) {
	int i;
	for (i = 0; curver[i] != who; i++) {
		if (alive[i]) //if he is still alive, increase his score
			increaseScore(i);
	}
	alive[i] = false;
	qDebug() << names[i] + " died";
	i++;
	for (; i < playercount; i++) {
		if (alive[i])
			increaseScore(i);
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
		qDebug() << names[alivePlayer] + " has won!";
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



void Game::setControls(int index, Qt::Key k, bool isRight) {
	controls[index][isRight] = k;
	qDebug() << "Assigned key to player "<< index <<": " + QKeySequence(k).toString();
}

void Game::nextRound() {
	nextRoundTimer->singleShot(roundTimeout, this, SLOT(startNextRound()));
}

void Game::startNextRound() {
	timer->stop();
	roundCount++;
	for (int i = 0; i < playercount; i++) {
		roundScore[i] = 0;
		QVariant returnedValue;
		QMetaObject::invokeMethod(qmlobject, "changeScore", Q_RETURN_ARG(QVariant, returnedValue), Q_ARG(QVariant, i) , Q_ARG(QVariant, score[i]), Q_ARG(QVariant, roundScore[i]));
		curver[i]->reset();
		alive[i] = true;
	}
	for (int i = 0; i < MAXITEMCOUNT; i++) {
		if (items[i] != NULL) {
			delete items[i];
			items[i] = NULL;
		}
	}
	lastItemSpawn = QTime::currentTime();
	timer->start();
}

void Game::setName(int index, QString newName) {
	names[index] = newName;
}

void Game::setRoundTimeout(int seconds) {
	roundTimeout = 1000*seconds;
}

void Game::setBaseSpeed(int baseSpeed) {
	this->baseSpeed = baseSpeed;
}

void Game::setQmlObject(QObject *o) {
	qmlobject = o;
}

void Game::increaseScore(int index) {
	score[index]++;
	roundScore[index]++;
	QVariant returnedValue;
	QMetaObject::invokeMethod(qmlobject, "changeScore", Q_RETURN_ARG(QVariant, returnedValue), Q_ARG(QVariant, index) , Q_ARG(QVariant, score[index]), Q_ARG(QVariant, roundScore[index]));
}

void Game::setAIcontrolled(int index, bool newState) {
	controlledByAI[index] = newState;
}
