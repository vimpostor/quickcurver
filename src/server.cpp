#include "server.h"

Server::Server(QCurver **curver, QObject *parent) : QObject(parent) {
	this->curver = curver;
	this->tcpReadyReadSignalMapper = new QSignalMapper(this); // create tcpReadyRead signal mapper
	connect(tcpReadyReadSignalMapper, SIGNAL(mapped(int)), this, SLOT(tcpReadyRead(int)));
	for (int i = 0; i < MAXPLAYERCOUNT; i++) {
		available[i] = false;
		clientsUdp[i] = NULL;
		clientsTcp[i] = NULL;
	}
}

void Server::init(quint16 port) {
	this->port = port;
	setServerIp();
	initUdpSocket();
	initTcpServer();
}

Server::~Server() {
	shutdown();
}

void Server::initUdpSocket() {
	const QHostAddress &h = *serverIp;
	udpSocket = new QUdpSocket(this);
	connect(udpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(udpSocketError(QAbstractSocket::SocketError)));
	connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
	udpSocket->bind(h, port);
}

void Server::initTcpServer() {
	tcpServer = new QTcpServer(this);
	if (!tcpServer->listen(QHostAddress::Any, port)) {
		qDebug() << "Unable to start the Tcp Server";
		return;
	}
	connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

void Server::readPendingDatagrams() {
	while (udpSocket->hasPendingDatagrams()) {
		QByteArray datagram;
		datagram.resize(udpSocket->pendingDatagramSize());
		QHostAddress *sender = new QHostAddress();
		quint16 senderPort;
		udpSocket->readDatagram(datagram.data(), datagram.size(), sender, &senderPort);
		QString msg = datagram;
		QString answer = "";

		if (msg == "[JOIN]") { //a player wants to join
			int i;
			for (i = 0; i < MAXPLAYERCOUNT && !(available[i] && clientsTcp[i] != NULL && clientsUdp[i] == NULL); ++i) {};
			if (i < MAXPLAYERCOUNT) {
				clientsUdp[i] = sender;
				answer = "[JOINED]";
				qDebug() << sender->toString() + " joined ";
				emit notifyGUI(sender->toString() + " joined", "SNACKBAR");
				emit playerStatusChanged(i, "JOINED");
			} else {
				answer = "[REJECTED]";
			}
		} else if (msg == "[LEFT]") {
			turn(sender, ROTATE_LEFT);
		} else if (msg == "[NONE]") {
			turn(sender, ROTATE_NONE);
		} else if (msg == "[RIGHT]") {
			turn(sender, ROTATE_RIGHT);
		} else {
			qDebug() << "Unsupported datagram arrived from " << sender->toString();
		}
		if (answer != "") {
			QByteArray answerDatagram;
			answerDatagram.append(answer);
			udpSocket->writeDatagram(answerDatagram, *sender, senderPort);
		}
	}
}

void Server::shutdown() {
	if (udpSocket != NULL) {
		udpSocket->close();
		disconnect(udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
	}
	if (tcpServer != NULL) {
		tcpServer->close();
	}
	for (int i = 0; i < MAXPLAYERCOUNT; ++i) {
		if (clientsTcp[i] != NULL) {
			clientsTcp[i]->close();
			disconnect(clientsTcp[i], SIGNAL(readyRead()), tcpReadyReadSignalMapper, SLOT(map()));
		}
	}
}

void Server::udpSocketError(QAbstractSocket::SocketError socketError) {
	if (socketError == QAbstractSocket::AddressInUseError) {
		qDebug() << "Cannot start the server, because the UDP socket is already used";
	} else {
		qDebug() << "An unhandled Udp socket error occured!\n" << socketError << udpSocket->errorString();
	}
}

int Server::isValidInput(QHostAddress *sender) {
	int i;
	for (i = 0; i < MAXPLAYERCOUNT && !(clientsUdp[i] != NULL && sender->toString() == clientsUdp[i]->toString()); ++i){}; //increment until out of boundaries or sender found
	return i < MAXPLAYERCOUNT ? i : -1;
}

void Server::turn(QHostAddress *sender, rotation r) {
	int i = isValidInput(sender);
	if (i != -1 && curver[i] != NULL) {
		curver[i]->rotating = r;
	}
}

void Server::setAvailable(int index, bool newState) {
	if (newState == false && clientsUdp[index] != NULL) {
		//kick player
		QByteArray datagram;
		datagram.append("KICKED");
		udpSocket->writeDatagram(datagram, *clientsUdp[index], 55225);
		clientsUdp[index] = NULL;
	}
	available[index] = newState;
}

void Server::start() {
	started = true;
	updateServerSettings();
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out << QString("[SETTINGS]") << serverSettings;
	sendToAllTcp(&block);
	transmitTcpMessage("[STARTED]");
	broadcastTimer = new QTimer(this);
	connect(broadcastTimer, SIGNAL(timeout()), this, SLOT(broadcast()));
	broadcastTimer->start(BROADCASTINTERVAL);
}

void Server::broadcast() {
	for (int i = 0; i < playercount; i++) {
		QCurver *c = curver[i];
		QByteArray datagram;
		QDataStream out(&datagram, QIODevice::WriteOnly);
		out << QString("HEAD") << i << c->getPos();
		sendToAllUdp(&datagram);
		datagram.clear();
		out.device()->reset();
		//now send curver data
		bool newSegment = c->moveToNextSegment(); //tries moving to next segment if all data was read from the last one
		out << QString("POS") << i << newSegment;
		for (int sentPoints = 0; sentPoints < 16 && c->hasUnsyncedSegPoints(); ++sentPoints) { //add points
			out << c->readUnsyncedSegPoint();
		}
		sendToAllUdp(&datagram);
	}
}

void Server::sendToAllUdp(QByteArray *datagram) {
	for (int i = 0; i < MAXPLAYERCOUNT; ++i) {
		if (clientsUdp[i] != NULL) {
			udpSocket->writeDatagram(*datagram, *clientsUdp[i], 55225);
		}
	}
}

void Server::setPlayerCount(int playercount) {
	this->playercount = playercount;
}

void Server::setServerIp() {
	QString ipAddress = Network::getLocalIpAddress().toString();
	serverIp = new QHostAddress(ipAddress);
	qDebug() << "Server set up on " + ipAddress;
}


void Server::newConnection() {
	QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
	connect(clientConnection, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(tcpSocketError(QAbstractSocket::SocketError)));
	connect(clientConnection, SIGNAL(disconnected()), clientConnection, SLOT(deleteLater()));
	(void) tryConnectingClient(clientConnection);
}

void Server::newRound() {
	transmitTcpMessage("[RESET]");
}

void Server::broadcastChatMessage(QString username, QString message) {
	if (username == "") {
		username = serverSettings.clientSettings[0].username;
	}
	emit sendMessage(username, message); // send to server itself
	// and send to others
	QString msgData[3];
	msgData[0] = "[MESSAGE]";
	msgData[1] = username;
	msgData[2] = message;
	transmitTcpMessages(msgData, 3);
}

int Server::getFreePlace() {
	int i;
	for (i = 0; i < MAXPLAYERCOUNT && !(available[i] && clientsUdp[i] == NULL); ++i) {} //increase counter until we find a suitable place
	return i < MAXPLAYERCOUNT ? i : -1;
}

void Server::transmitTcpMessage(QString message, QTcpSocket *s) {
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out << message;
	if (s == NULL) { // send to all
		sendToAllTcp(&block);
	} else { // send only to s
		s->write(block);
	}
}

void Server::transmitTcpMessages(QString *messages, int length, QTcpSocket *s) {
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	for (int i = 0; i < length; ++i) {
		out << messages[i];
	}
	if (s == NULL) { // send to all
		sendToAllTcp(&block);
	} else {
		s->write(block); // send only to s
	}
}

void Server::sendToAllTcp(QByteArray *block) {
	for (int i = 0; i < MAXPLAYERCOUNT; ++i) {
		if (clientsTcp[i] != NULL) {
			clientsTcp[i]->write(*block);
		}
	}
}

bool Server::tryConnectingClient(QTcpSocket *client) {
	int index = getFreePlace();
	if (index != -1) {
		connectClient(client, index);
		transmitTcpMessage("[ACCEPTED]", client);
		return true;
	} else {
		transmitTcpMessage("[REJECTED]", client);
		return false;
	}
}

void Server::connectClient(QTcpSocket *client, int index) {
	clientsTcp[index] = client;
	connect(client, SIGNAL(readyRead()), tcpReadyReadSignalMapper, SLOT(map()));
	tcpReadyReadSignalMapper->setMapping(client, index);
//    in[index].abortTransaction();
	in[index].setDevice(client);
}

void Server::tcpReadyRead(int i) {
	in[i].startTransaction();
	QString message;
	in[i] >> message;
	if (!in[i].commitTransaction()) {
		return;
	}

	if (message == "[MESSAGE]") {
		QString username, message;
		in[i] >> username >> message;
		broadcastChatMessage(username, message);
	} else if (message == "[SETTINGS]") {
		in[i] >> serverSettings.clientSettings[i];
		emit playerStatusChanged(i, serverSettings.clientSettings[i].ready ? "READY" : "UNREADY");
		emit playerStatusChanged(i, "USERNAME" + serverSettings.clientSettings[i].username);
	} else if (message == "[LEFT]") {
		disconnectClient(clientsTcp[i]);
	} else {
		qDebug() << "Unsupported tcp message arrived on server";
		qDebug() << message;
	}
}

void Server::disconnectClient(QTcpSocket *client, QString reason) {
	for (int i = 0; i < MAXPLAYERCOUNT; ++i) {
		if (clientsTcp[i] != NULL && clientsTcp[i]->peerAddress().isEqual(client->peerAddress())) {
			// remove tcp
			if (reason != "") { // notify if reason is specified
				transmitTcpMessage(reason, clientsTcp[i]);
			}
			disconnect(client, SIGNAL(readyRead()), tcpReadyReadSignalMapper, SLOT(map()));
			clientsTcp[i]->close();
			disconnect(clientsTcp[i], SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(tcpSocketError(QAbstractSocket::SocketError)));
//            delete clientsTcp[i]; // do not delete, the socket will be automatically deleted using deleteLater signal-slot
			clientsTcp[i] = NULL;
			clientsUdp[i] = NULL; // remove udp
			in[i].setDevice(NULL); // reset datastream
			in[i].resetStatus();
			QString msg = serverSettings.clientSettings[i].username + " left the match";
			if (started) {
				broadcastChatMessage("Chat Bot", msg);
			} else {
				notifyGUI(msg, "SNACKBAR");
			}
			serverSettings.clientSettings[i].reset();
			emit playerStatusChanged(i, "LEFT");
		}
	}
}

void Server::tcpSocketError(QAbstractSocket::SocketError socketError) {
	QTcpSocket *s = (QTcpSocket *) sender();
	if (socketError == QAbstractSocket::RemoteHostClosedError) {
		disconnectClient(s); // disconnect client then if the connection was closed
	} else {
		qDebug() << "An unhandled Tcp socket error occured!\n" << socketError;
	}
}

QHostAddress *Server::getServerIp() {
	return serverIp;
}

bool Server::isReady() {
	for (int i = 0; i < MAXPLAYERCOUNT; ++i) {
		if (available[i]) {
			if (clientsTcp[i] == NULL) {
				emit notifyGUI("There is an unused online slot! Please remove it or let someone join.", "SNACKBAR");
				return false;
			} else if (clientsUdp[i] == NULL) {
				emit notifyGUI("One of the clients has not successfully joined yet!", "SNACKBAR");
				return false;
			} else if (!serverSettings.clientSettings[i].ready) {
				emit notifyGUI("One of the clients is not ready yet!", "SNACKBAR");
				return false;
			} else if (serverSettings.clientSettings[i].username == "") {
				emit notifyGUI("One of the clients hasn't set the username", "SNACKBAR");
				return false;
			}
		}
	}
	return true;
}

void Server::transmitNewItem(QString iconName, QColor color, QPointF pos, int index) {
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out << QString("[ITEM]") << iconName << color << pos << index;
	sendToAllTcp(&block);
}

void Server::useItem(int index) {
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out << QString("[ITEMUSED]") << index;
	sendToAllTcp(&block);
}

void Server::cleanInstall() {
	transmitTcpMessage("[CLEANINSTALL]");
}

void Server::curverDied(int index) {
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out << QString("[DEATH]") << index;
	sendToAllTcp(&block);
}

void Server::updateServerSettings() {
	serverSettings.playerCount = this->playercount;
	for (int i = 0; i < playercount; ++i) {
		serverSettings.colors[i] = curver[i]->getColor();
	}
}

void Server::setName(int index, QString username) {
	serverSettings.clientSettings[index].username = username;
}
