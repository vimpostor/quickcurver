#include "server.h"

Server::Server(QCurver **curver, quint16 port, QObject *parent) : QObject(parent) {
    this->port = port;
    this->curver = curver;
    for (int i = 0; i < MAXPLAYERCOUNT; i++) {
        available[i] = false;
        clients[i] = NULL;
        currentSegment[i] = 0;
        currentPos[i] = 0;
    }
    setServerIp();
    initUdpSocket();
//    initTcpServer();
}

Server::~Server() {
	shutdown();
}

void Server::initUdpSocket() {
    const QHostAddress &h = *serverIp;
	udpSocket = new QUdpSocket(this);
	connect(udpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
	connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
    udpSocket->bind(h, port);
}

void Server::initTcpServer() {
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen()) {
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
		if (msg == "JOIN") { //a player wants to join
			bool success = false;
			//check if there is a curver available to take control of
			int i;
			for (i = 0; i < MAXPLAYERCOUNT && !(available[i] && clients[i] == NULL); i++) {} //increase counter until we find a suitable place
			if (i < MAXPLAYERCOUNT) { //if a place is free
				clients[i] = sender;
				success = true;
			}
			answer = success ? "JOINED" : "REJECTED";
			qDebug() << sender->toString() + " " + answer;
			if (answer == "JOINED" && started) {
				answer = "HOTJOINED";
			}
		} else if (msg == "LEAVE") {
			int i = isValidInput(sender);
			if (i != -1) {
				clients[i] = NULL;
			}
		} else if (msg == "LEFT") {
			turn(sender, ROTATE_LEFT);
		} else if (msg == "NONE") {
			turn(sender, ROTATE_NONE);
		} else if (msg == "RIGHT") {
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
}

void Server::socketError(QAbstractSocket::SocketError socketError) {
	qDebug() << "A socket error occured!\n" << socketError << udpSocket->errorString();
}

int Server::isValidInput(QHostAddress *sender) {
	int i;
	for (i = 0; i < MAXPLAYERCOUNT && !(clients[i] != NULL && sender->toString() == clients[i]->toString()); i++); //increment until out of boundaries or sender found
	return i < MAXPLAYERCOUNT ? i : -1;
}

void Server::turn(QHostAddress *sender, rotation r) {
	int i = isValidInput(sender);
	if (i != -1 && curver[i] != NULL) {
		curver[i]->rotating = r;
	}
}

void Server::setAvailable(int index, bool newState) {
	if (newState == false && clients[index] != NULL) {
		//kick player
		QByteArray datagram;
		datagram.append("KICKED");
		udpSocket->writeDatagram(datagram, *clients[index], 55225);
		clients[index] = NULL;
	}
	available[index] = newState;
}

void Server::start() {
	started = true;
	QByteArray datagram;
	datagram.append("STARTED");
	sendToAll(&datagram);
	broadcastTimer = new QTimer(this);
	connect(broadcastTimer, SIGNAL(timeout()), this, SLOT(broadcast()));
	broadcastTimer->start(BROADCASTINTERVAL);
}

void Server::broadcast() {
	for (int i = 0; i < playercount; i++) {
		QCurver *c = curver[i];
		QByteArray datagram;
        QPointF pos = c->getPos();
        QDataStream out(&datagram, QIODevice::WriteOnly);
        out << QString("HEAD") << i << pos;
        sendToAll(&datagram);
//		datagram.append("POS" + i + ":" + currentSegment[i] + ":");
	}
}

void Server::sendToAll(QByteArray *datagram) {
	for (int i = 0; i < MAXPLAYERCOUNT; i++) {
		if (clients[i] != NULL) {
			udpSocket->writeDatagram(*datagram, *clients[i], 55225);
		}
	}
}

void Server::setPlayerCount(int playercount) {
	this->playercount = playercount;
}

void Server::setServerIp() {
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost && ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

    serverIp = new QHostAddress(ipAddress);
    qDebug() << "Server set up on " + ipAddress;
}


void Server::newConnection() {
    qDebug() << "Someone connected on Tcp";
    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection, SIGNAL(disconnected()), clientConnection, SLOT(deleteLater()));
}
