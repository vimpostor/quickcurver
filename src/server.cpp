#include "server.h"

Server::Server(QCurver **curver, quint16 port, QObject *parent) : QObject(parent) {
	this->port = port;
	this->curver = curver;
	for (int i = 0; i < 16; i++) {
		available[i] = false;
		clients[i] = NULL;
	}
	initSocket();
}

Server::~Server() {
	shutdown();
}

void Server::initSocket() {
	udpSocket = new QUdpSocket(this);
	connect(udpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
	connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
	udpSocket->bind(QHostAddress::LocalHost, port);
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
			for (i = 0; i < 16 && !(available[i] && clients[i] == NULL); i++) {} //increase counter until we find a suitable place
			if (i < 16) { //if a place is free
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
	udpSocket->close();
	disconnect(udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
}

void Server::socketError(QAbstractSocket::SocketError socketError) {
	qDebug() << "A socket error occured!\n" << socketError << udpSocket->errorString();
}

int Server::isValidInput(QHostAddress *sender) {
	int i;
	for (i = 0; i < 16 && !(clients[i] != NULL && sender->toString() == clients[i]->toString()); i++); //increment until out of boundaries or sender found
	return i < 16 ? i : -1;
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
	for (int i = 0; i < 16; i++) {
		if (clients[i] != NULL) {
			udpSocket->writeDatagram(datagram, *clients[i], 55225);
		}
	}
}
