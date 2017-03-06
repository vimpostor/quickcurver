#include "game.h"

Game::Game(QQuickItem *parent) : QQuickItem(parent) {
	segment::initRand();
	for (int i = 0; i < MAXPLAYERCOUNT; i++) {
		controls[i][0] = Qt::Key_Left;
		controls[i][1] = Qt::Key_Right;
		controlledByAI[i] = false;
		controlledByNetwork[i] = false;
	}
    server = new Server(curver, 52552, this);
    client = new Client(this);
    connect(server, SIGNAL(notifyGUI(QString,QString)), this, SLOT(notifyGUI(QString,QString)));
    connect(server, SIGNAL(playerStatusChanged(int,QString)), this, SLOT(setPlayerStatus(int,QString)));
}

Game::~Game() {
    if (timer != NULL) {
        timer->stop();
    }
    if (node != NULL) {
        node->removeAllChildNodes();
        delete node;
    }
    if (server != NULL) {
        delete server;
    }
    if (client != NULL) {
        delete client;
    }
}

QSGNode *Game::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) {
	(void) oldNode; //suppresses unused variable warning
	return node;
}

void Game::start() {
    timer = new QTimer(this);
    nextRoundTimer = new QTimer(this);
    node = new QSGNode;
    setFlag(ItemHasContents);
    wall = new wallNode(node, fieldsize);
    for (int i = 0; i < ITEMVARIETY; i++) {
        itemPrioritySum += itemPriority[i];
    }
    for (int i = 0; i < playercount; i++) {
        score[i] = 0;
        roundScore[i] = 0;
        curver[i] = new QCurver(node, colors[i], baseSpeed, fieldsize);
        if (controlledByAI[i]) {
            ai[i] = new AIController(curver[i], curver, playercount, fieldsize);
        }
        connect(curver[i], SIGNAL(died(QCurver*)), this, SLOT(curverDied(QCurver*)));
        connect(curver[i], SIGNAL(requestIntersectionChecking(QPointF,QPointF)), this, SLOT(checkforIntersection(QPointF,QPointF)));
    }
    for (int i = 0; i < MAXITEMCOUNT; i++) {
        items[i] = NULL;
    }
    connect(timer, SIGNAL(timeout()), this, SLOT(progress()));
    lastTime = QTime::currentTime();
    lastItemSpawn = lastTime;
    nextItemSpawn = segment::randInt(5000,10000);
    server->setPlayerCount(playercount);
    connect(server, SIGNAL(sendMessage(QString,QString)), this, SLOT(sendMessageToQml(QString,QString)));
    server->start();
    timer->start(timerInterval);
}

void Game::clientStart(QString ip, int port) {
    server->shutdown();
    isHost = false;
    node = new QSGNode;
    wall = new wallNode(node, fieldsize);
    connect(client, SIGNAL(joinStatusChanged(QString)), this, SLOT(setJoinStatus(QString)));
    connect(client, SIGNAL(updateGUI()), this, SLOT(updateGUI()));
    connect(client, SIGNAL(sendMessage(QString,QString)), this, SLOT(sendMessageToQml(QString,QString)));
    connect(client, SIGNAL(spawnItem(QString,QColor,QPointF,int)), this, SLOT(clientSpawnItem(QString,QColor,QPointF,int)));
    connect(client, SIGNAL(deleteItem(int)), this, SLOT(clientDeleteItem(int)));
    connect(client, SIGNAL(deleteAllItems()), this, SLOT(clientDeleteAllItems()));
    client->start(node, ip, port);
    setFlag(ItemHasContents);
}

void Game::setColor(int index, QColor color) {
	colors[index] = color;
}

void Game::progress() {
	//calculate time since last progress()
    float deltat = (float) lastTime.msecsTo(QTime::currentTime())/1000 * effectiveTimeMultiplier;
    if (deltat != 0) {
        lastTime = QTime::currentTime();
        if (itemSpawnrate != 0 && lastItemSpawn.msecsTo(lastTime) > nextItemSpawn) { // try spawning item
            int i;
            for (i = 0; i < MAXITEMCOUNT && items[i] != NULL; i++) { //find first free item slot
            }
            if (i == MAXITEMCOUNT) { // no free slot
                qDebug() << "No free slot for a new item, will try spawning later...";
                nextItemSpawn = segment::randInt(100000/itemSpawnrate,1000000/itemSpawnrate); // dont worry this is only entered, when itemSpawnrate != 0
            } else { // free slot at i
                int r = segment::randInt(1, itemPrioritySum);
                int itemSelector = 0;
                for (itemSelector = 0; r > 0; r -= itemPriority[itemSelector], itemSelector++) {};
                itemSelector--;
                switch (itemSelector) {
                case 0:
                    items[i] = new FastItem(node, &textureGenerator, fieldsize);
                    break;
                case 1:
        //			items[i] = new SlowItem(node);
                    break;
                case 2:
                    items[i] = new CleaninstallItem(node, &textureGenerator, fieldsize, server);
                    break;
                case 3:
                    //global wall hack
                    break;
                case 4:
                    //invisibility
                    break;
                case 5:
                    items[i] = new FatterItem(node, &textureGenerator, fieldsize);
                    break;
                default:
                    qDebug() << "(EE) This should not happen, the algorithm should always be able to decide what item to spawn";
                    break;
                }
                items[i]->setRound(roundCount);
                server->transmitNewItem(items[i]->getIconName(), items[i]->getColor(), items[i]->getPos(), i);
                nextItemSpawn = segment::randInt(10000/itemSpawnrate,100000/itemSpawnrate); // dont worry this is only entered, when itemSpawnrate != 0
            }
            lastItemSpawn = lastTime;
        }
        for (int i = 0; i < playercount; i++) {
            if (curver[i]->alive) {
                //check for item collision
                for (int j = 0; j < MAXITEMCOUNT; j++) {
                    if (items[j] != NULL && items[j]->testCollision(curver[i]->getPos())) {
                        //use item
                        items[j]->useItem(playercount, curver, curver[i]);
                        items[j] = NULL; //dont worry it will delete it by its own
                        server->useItem(j);
                    }
                }
                //let the AI make its move now
                if (controlledByAI[i] && ((frameCount+i) % AIINTERVAL == 0)) {
                    ai[i]->makeMove(deltat);
                }
                curver[i]->progress(deltat);
            }
        }
        frameCount++;
        update();
    }
}

void Game::sendKey(Qt::Key k) {
    if (isHost) {
		for (int i = 0; i < playercount; i++) {
			if (!controlledByAI[i] && !controlledByNetwork[i]) {
				if (controls[i][0] == k) {
					curver[i]->rotating = ROTATE_LEFT;
				} else if (controls[i][1] == k) {
					curver[i]->rotating = ROTATE_RIGHT;
				}
			}
		}
	} else { //client
		client->sendKey(k);
	}
}

void Game::releaseKey(Qt::Key k) {
    if (isHost) {
		for (int i = 0; i < playercount; i++) {
			if (!controlledByAI[i] && !controlledByNetwork[i]) {
				if (controls[i][0] == k || controls[i][1] == k) {
					curver[i]->rotating = ROTATE_NONE;
				}
			}
		}
	} else { //client
		client->releaseKey(k);
	}
}

void Game::addPlayer() {
	playercount++;
}

void Game::curverDied(QCurver *who) {
    int index; // first get the index of the curver that died
    for (index = 0; index < playercount && curver[index] != who; ++index) {
    }
    if (curver[index]->alive) { // todo: fix "curverDied" being called multiple times
        int i;
        bool onlyBotsAlive = true;
        server->curverDied(index);
        for (i = 0; curver[i] != who; i++) {
            if (curver[i]->alive) //if he is still alive, increase his score
                increaseScore(i);
        }
        curver[i]->alive = false;
//        server->broadcastChatMessage("Chat Bot", names[i] + " died");
        i++;
        for (; i < playercount; ++i) {
            if (curver[i]->alive)
                increaseScore(i);
        }
        int stillAlive = 0;
        int alivePlayer;
        for (i = 0; i < playercount; i++) {
            if (curver[i]->alive) {
                stillAlive++;
                alivePlayer = i;
                if (!controlledByAI[i]) {
                    onlyBotsAlive = false;
                }
            }
        }
        if (onlyBotsAlive) {
            effectiveTimeMultiplier = timeMultiplier;
        }
        if (stillAlive == 1) {
            if (sendWinnerMessages) {
                server->broadcastChatMessage("Chat Bot", names[alivePlayer] + " has won!");
            }
            nextRound();
        }
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
}

void Game::nextRound() {
    effectiveTimeMultiplier = 1;
	nextRoundTimer->singleShot(roundTimeout, this, SLOT(startNextRound()));
}

void Game::startNextRound() {
    effectiveTimeMultiplier = 1;
	timer->stop();
    server->newRound();
	roundCount++;
	for (int i = 0; i < playercount; i++) {
		roundScore[i] = 0;
		QVariant returnedValue;
		QMetaObject::invokeMethod(qmlobject, "changeScore", Q_RETURN_ARG(QVariant, returnedValue), Q_ARG(QVariant, i) , Q_ARG(QVariant, score[i]), Q_ARG(QVariant, roundScore[i]));
		curver[i]->reset();
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
    server->setName(index, newName);
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

void Game::sendMessageToQml(QString sender, QString message) {
    QVariant returnedValue;
    QMetaObject::invokeMethod(qmlobject, "sendMessage", Q_RETURN_ARG(QVariant, returnedValue), Q_ARG(QVariant, sender), Q_ARG(QVariant, message));
}


void Game::setController(int index, int newControllerState) {
	controlledByAI[index] = (newControllerState == 1);
	controlledByNetwork[index] = (newControllerState == 2);
	server->setAvailable(index, controlledByNetwork[index]);
}

void Game::setItemPriority(int index, int newPriority) {
	itemPriority[index] = newPriority;
}

void Game::setItemSpawnrate(int value) {
    itemSpawnrate = value;
}

void Game::setJoinStatus(QString s) {
	QVariant returnedValue;
	QMetaObject::invokeMethod(qmlobject, "setJoinStatus", Q_RETURN_ARG(QVariant, returnedValue), Q_ARG(QVariant, s));
}

void Game::close() {
	delete this;
}

void Game::updateGUI() {
    update();
}

void Game::setFieldSize(int s) {
    fieldsize = s;
}

void Game::setPlayerStatus(int index, QString s) {
    QVariant returnedValue;
    QMetaObject::invokeMethod(qmlobject, "setPlayerStatus", Q_RETURN_ARG(QVariant, returnedValue), Q_ARG(QVariant, index), Q_ARG(QVariant, s));
}

void Game::setTimeMultiplier(int t) {
    timeMultiplier = t;
}

void Game::requestSendMessage(QString message) {
    if (isHost) {
        server->broadcastChatMessage(username, message);
    } else {
        client->requestSendMessage(message);
    }
}

void Game::setSendWinnerMessages(bool checked) {
    sendWinnerMessages = checked;
}

void Game::notifyGUI(QString msg, QString mode) {
    QVariant returnedValue;
    QMetaObject::invokeMethod(qmlobject, "notifyGUI", Q_RETURN_ARG(QVariant, returnedValue), Q_ARG(QVariant, msg), Q_ARG(QVariant, mode));
}

bool Game::isReady() {
    return server->isReady();
}

void Game::changeClientSettings(QString username, bool ready) {
    client->changeSettings(username, ready);
}

void Game::leaveGame() {
    if (!isHost) {
        client->shutdown();
        notifyGUI("You left the game!", "SNACKBAR");
        isHost = true;
    }
}

void Game::clientSpawnItem(QString iconName, QColor color, QPointF pos, int index) {
    if (items[index] != NULL) {
        qDebug() << "Item is not NULL in clientSpawnItem()";
    } else {
        items[index] = new CurveItem(node, &textureGenerator, iconName, color, pos);
    }
}

QString Game::getClipboardContent() {
    return QGuiApplication::clipboard()->text();
}

QString Game::copyIp() {
    QString ip = server->getServerIp()->toString();
    QGuiApplication::clipboard()->setText(ip);
    return ip;
}

void Game::clientDeleteItem(int index) {
    if (items[index] == NULL) {
        qDebug() << "Item not spawned in the first place";
    } else {
        delete items[index];
        items[index] = NULL;
    }
}

void Game::clientDeleteAllItems() {
    for (int i = 0; i < MAXITEMCOUNT; ++i) {
        if (items[i] != NULL) {
            delete items[i];
            items[i] = NULL;
        }
    }
}

int Game::getFieldSize() {
    return this->fieldsize;
}
