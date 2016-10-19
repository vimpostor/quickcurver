#include "client.h"

Client::Client(QString ip, QSGNode *node, int port, QObject *parent) : QObject(parent) {
    this->ip = new QHostAddress(ip);
    this->port = port;
    this->node = node;
    timeoutTimer = new QTimer;
    initSocket();
    join();
    QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
    material->setColor(Qt::red);
    headnode = new headNode(QPointF(0,0), material, node);
}

Client::~Client() {
	shutdown();
}

void Client::sendKey(Qt::Key k) {
	QString msg = "";
	if (k == Qt::Key_Left) {
		msg = "LEFT";
	} else if (k == Qt::Key_Right) {
		msg = "RIGHT";
	}
	if (msg != "") {
		QByteArray datagram;
		datagram.append(msg);
		udpSocket->writeDatagram(datagram, *ip, port);
	}
}

void Client::releaseKey(Qt::Key k) {
	QByteArray datagram;
	datagram.append("NONE");
	udpSocket->writeDatagram(datagram, *ip, port);
}

void Client::initSocket() {
	udpSocket = new QUdpSocket(this);
	connect(udpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
	connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
	udpSocket->bind(*ip, myport);
}

void Client::readPendingDatagrams() {
	while (udpSocket->hasPendingDatagrams()) {
		QByteArray datagram;
		datagram.resize(udpSocket->pendingDatagramSize());
		QHostAddress *sender = new QHostAddress();
		quint16 senderPort;
		udpSocket->readDatagram(datagram.data(), datagram.size(), sender, &senderPort);
		QString msg = datagram;
//		QString answer = "";
		if (msg == "JOINED") { //we successfully joined
			joined = true;
			emit joinStatusChanged("JOINED");
		} else if (msg == "REJECTED") {
			emit joinStatusChanged("REJECTED");
			shutdown();
		} else if (msg == "STARTED") {
			emit joinStatusChanged("STARTED");
		} else if (msg == "HOTJOINED") {
			emit joinStatusChanged("STARTED");
		} else if (msg == "KICKED") {
			emit joinStatusChanged(("KICKED"));
        } else if (msg.startsWith("HEAD")) { //head data comes in
            qDebug() << msg;
        } else if (msg.startsWith("POS")) {
			//new data comes in
		} else {
            //try reading stream
            QString title;
            QDataStream in(&datagram, QIODevice::ReadOnly);
            in >> title;
            if (title == "HEAD") {
                int i;
                QPointF pos;
                in >> i >> pos; //index of curver and position of its head
                headnode->updatePosition(pos);
                emit updateGUI();
            } else {
                qDebug() << "Server replied with unsupported datagram";
            }
		}
//		if (answer != "") {
//			QByteArray answerDatagram;
//			answerDatagram.append(answer);
//			udpSocket->writeDatagram(answerDatagram, *sender, senderPort);
//		}
	}
}

void Client::shutdown() {
    if (udpSocket != NULL) {
        udpSocket->close();
        disconnect(udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
    }
}

void Client::socketError(QAbstractSocket::SocketError socketError) {
	qDebug() << "A socket error occured!\n" << socketError << udpSocket->errorString();
}

void Client::join() {
	QByteArray datagram;
	datagram.append("JOIN");
	udpSocket->writeDatagram(datagram, *ip, port);
	timeoutTimer->singleShot(5000, this, SLOT(timeout()));
}

void Client::timeout() {
	if (!joined) {
		emit joinStatusChanged("TIMEOUT");
		shutdown();
	}
}
