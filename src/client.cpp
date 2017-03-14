#include "client.h"

Client::Client(QQuickItem *parent) : QObject(parent) {
	this->parent = parent;
	timeoutTimer = new QTimer;
	for (int i = 0; i < MAXPLAYERCOUNT; i++) {
		headnodes[i] = NULL;
		curver[i] = NULL;
	}
}

Client::~Client() {
	shutdown();
}

void Client::start(QSGNode *node, QObject *qmlobject, QString ip, int port) {
	this->node = node;
	this->qmlobject = qmlobject;
	this->ip = new QHostAddress(ip);
	this->port = port;
	gui.setQmlObject(qmlobject);
	initTcpSocket();
	initUdpSocket();
	join();
}

void Client::shutdown() {
	if (udpSocket != NULL) {
		udpSocket->close();
		disconnect(udpSocket, SIGNAL(readyRead()), this, SLOT(udpReadPendingDatagrams()));
	}
	if (tcpSocket != NULL) {
		if (tcpSocket->isOpen()) {
			sendTcpMessage("[LEFT]");
		}
		tcpSocket->close();
		disconnect(tcpSocket, SIGNAL(readyRead()), this, SLOT(tcpReadyRead()));
	}
}

void Client::sendKey(Qt::Key k) {
	QString msg = "";
	if (k == Qt::Key_Left) {
		msg = "[LEFT]";
	} else if (k == Qt::Key_Right) {
		msg = "[RIGHT]";
	}
	if (msg != "") {
		sendUdpMessage(msg);
	}
}

void Client::releaseKey(Qt::Key k) {
	(void) k; // suppresses unused variable warning
	sendUdpMessage("[NONE]");
}

void Client::initUdpSocket() {
	udpSocket = new QUdpSocket(this);
	connect(udpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(udpSocketError(QAbstractSocket::SocketError)));
	connect(udpSocket, SIGNAL(readyRead()), this, SLOT(udpReadPendingDatagrams()));
	udpSocket->bind(Network::getLocalIpAddress(), myport);
}

void Client::initTcpSocket() {
	tcpSocket = new QTcpSocket(this);
	tcpSocket->abort();
	connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(tcpSocketError(QAbstractSocket::SocketError)));
	connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(tcpReadyRead()));
	in.setDevice(tcpSocket);
}

void Client::udpReadPendingDatagrams() {
	while (udpSocket->hasPendingDatagrams()) {
		QByteArray datagram;
		datagram.resize(udpSocket->pendingDatagramSize());
		QHostAddress *sender = new QHostAddress();
		quint16 senderPort;
		udpSocket->readDatagram(datagram.data(), datagram.size(), sender, &senderPort);
		QString msg = datagram;

		if (msg == "[JOINED]") { //we successfully joined
			joined = true;
			emit setJoinStatus("JOINED");
			changeSettings(settings.username, settings.ready); // send settings to server
		} else {
			//try reading stream
			QString title;
			QDataStream in(&datagram, QIODevice::ReadOnly);
			in >> title;
			if (title == "HEAD") {
				int i;
				QPointF pos;
				in >> i >> pos; //index of curver and position of its head
				if (headnodes[i] == NULL) { //create new curver
					qDebug() << "This should not happen! Headnode is NULL!";
				} else {
					headnodes[i]->updatePosition(pos);
				}
				parent->update();
			} else if (title == "POS") {
				int i, amount;
				bool newSegment;
				QPointF pos;
				in >> i >> newSegment >> amount;
				if (newSegment) {
					curver[i]->clientNewSegment();
				}
				for (int receivedPoints = 0; receivedPoints < amount; ++receivedPoints) {
					in >> pos;
					curver[i]->clientAddPoint(pos);
				}
			} else {
				qDebug() << "Server replied with unsupported datagram";
			}
		}
	}
}

void Client::udpSocketError(QAbstractSocket::SocketError socketError) {
	qDebug() << "An UDP socket error occured!\n" << socketError << udpSocket->errorString();
}

void Client::join() {
	tcpSocket->connectToHost(*ip, port); // attempts to establish a connection to the server
	timeoutTimer->singleShot(5000, this, SLOT(timeout()));
}

void Client::timeout() {
	if (!joined) {
		emit setJoinStatus("TIMEOUT");
		shutdown();
	}
}

void Client::tcpReadyRead() {
	while (!tcpSocket->atEnd()) {
		in.startTransaction();
		QString message;
		in >> message;
		if (!in.commitTransaction()) {
			return;
		}

		if (message == "[ACCEPTED]") {
			sendUdpMessage("[JOIN]"); // test udp connection as well
			emit setJoinStatus("TCPACK");
		} else if (message == "[REJECTED]") {
			emit setJoinStatus("REJECTED");
		} else if (message == "[MESSAGE]") {
			QString username, message;
			in >> username >> message;
			gui.sendChatMessage(username, message);
		} else if (message == "[STARTED]") {
			emit setJoinStatus("STARTED");
		} else if (message == "[ITEM]") {
			QString iconName;
			QColor color;
			QPointF pos;
			int index;
			in >> iconName >> color >> pos >> index;
			emit spawnItem(iconName, color, pos, index);
		} else if (message == "[RESET]") {
			for (int i = 0; i < MAXPLAYERCOUNT; ++i) {
				if (curver[i] != NULL) {
					curver[i]->clientReset();
					gui.setPlayerScore(i, curver[i]->score, curver[i]->roundScore);
				}
			}
			emit deleteAllItems();
		} else if (message == "[ITEMUSED]") {
			int index;
			in >> index;
			emit deleteItem(index);
		} else if (message == "[CLEANINSTALL]") {
			for (int i = 0; i < MAXPLAYERCOUNT; ++i) {
				if (curver[i] != NULL) {
					curver[i]->cleanInstall();
				}
			}
		} else if (message == "[DEATH]") {
			int index;
			in >> index;
			curver[index]->die();
			for (int i = 0; i < MAXPLAYERCOUNT; ++i) {
				if (curver[i] != NULL && curver[i]->alive) {
					curver[i]->increaseScore();
					gui.setPlayerScore(i, curver[i]->score, curver[i]->roundScore);
				}
			}
		} else if (message == "[SETTINGS]") {
			in >> serverSettings;
			// create curvers
			for (int i = 0; i < serverSettings.playerCount; ++i) {
				gui.editUsername(i, serverSettings.clientSettings[i].username);
				QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
				material->setColor(serverSettings.colors[i]);
				headnodes[i] = new headNode(QPointF(), material, node);
				curver[i] = new QCurver(node, serverSettings.colors[i]);
			}
		} else {
			qDebug() << "Unsupported tcp message arrived on client";
		}
	}
}

void Client::tcpSocketError(QAbstractSocket::SocketError socketError) {
	if (socketError == QAbstractSocket::RemoteHostClosedError) {
		// leave the game then
		tcpSocket->close();
		qDebug() << "Terminating, because the server closed the connection";
		emit setJoinStatus("TERMINATE"); // TODO: this should be handled more gracefully
	} else {
		qDebug() << "An unhandled TCP socket error occured!\n" << socketError << tcpSocket->errorString();
	}
}

void Client::sendUdpMessage(QString msg) {
	QByteArray datagram;
	datagram.append(msg);
	udpSocket->writeDatagram(datagram, *ip, port);
}

void Client::sendTcpMessage(QString msg) {
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out << msg;
	tcpSocket->write(block);
}

void Client::requestSendMessage(QString message) {
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out << QString("[MESSAGE]") << settings.username << message;
	tcpSocket->write(block);
}

void Client::changeSettings(QString username, bool ready) {
	settings.username = username;
	settings.ready = ready;
	if (joined) {
		QByteArray block;
		QDataStream out(&block, QIODevice::WriteOnly);
		out << QString("[SETTINGS]") << settings;
		tcpSocket->write(block);
	}
}

void Client::setJoinStatus(QString s) {
	QVariant returnedValue;
	QMetaObject::invokeMethod(qmlobject, "setJoinStatus", Q_RETURN_ARG(QVariant, returnedValue), Q_ARG(QVariant, s));
}
