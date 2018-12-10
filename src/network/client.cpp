#include "client.h"

Client::Client()
{
	in.setDevice(&tcpSocket);
	connect(&tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &Client::socketError);
	connect(&tcpSocket, &QTcpSocket::connected, this, &Client::socketConnected);
	connect(&tcpSocket, &QTcpSocket::disconnected, this, &Client::socketDisconnected);
	connect(&tcpSocket, &QTcpSocket::readyRead, this, &Client::socketReadyRead);
	connect(&udpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &Client::udpSocketError);
	connect(&udpSocket, &QUdpSocket::readyRead, this, &Client::udpSocketReadyRead);

	// choose an arbitrary local port for UDP
	udpSocket.bind();
}

/**
 * @brief Returns the current join status
 * @return joinStatus
 */
Client::JoinStatus Client::getJoinStatus() const
{
	return this->joinStatus;
}

/**
 * @brief Connects the Client to the given host
 * @param addr The IP address of the host to connect to
 * @param port The port that the host is listening on
 */
void Client::connectToHost(QString addr, quint16 port)
{
	// first look up the hostname
	setJoinStatus(JoinStatus::DNS_PENDING);
	// blocking call
	QHostInfo info = QHostInfo::fromName(addr);
	if (info.error()) {
		setJoinStatus(JoinStatus::FAILED);
		Gui::getSingleton().postInfoBar(info.errorString());
		return;
	} else if (info.addresses().isEmpty()) {
		setJoinStatus(JoinStatus::FAILED);
		Gui::getSingleton().postInfoBar("Could not resolve hostname");
		return;
	}
	QHostAddress hostAddr = info.addresses().first();
	this->serverAddress = {hostAddr, port};
	setJoinStatus(JoinStatus::TCP_PENDING);
	tcpSocket.connectToHost(hostAddr, port);
}

/**
 * @brief Sends a chat message for the Server to broadcast
 * @param msg The chat message
 */
void Client::sendChatMessage(QString msg)
{
	Packet::ClientChatMsg p;
	p.message = msg;
	p.sendPacket(&tcpSocket);
}

/**
 * @brief Sends the PlayerModel of the Client to the Server
 */
void Client::sendPlayerModel()
{
	Packet::ClientPlayerModel p;
	p.username = Settings::getSingleton().getClientName();
	p.color = Settings::getSingleton().getClientColor();
	p.sendPacket(&tcpSocket);
}

/**
 * @brief Processes the key
 *
 * If the key matches, this changes the rotation of the Curver belonging to the Client
 * @param key The key to process
 * @param release Whether the key was pressed or released
 */
void Client::processKey(Qt::Key key, bool release)
{
	Packet::ClientCurverRotation p;
	if (release) {
		p.rotation = Curver::Rotation::ROTATE_NONE;
	} else {
		// TODO: Allow custom keys
		if (key == Qt::Key_Left)
			p.rotation = Curver::Rotation::ROTATE_LEFT;
		else
			p.rotation = Curver::Rotation::ROTATE_RIGHT;
	}
	p.sendPacket(&tcpSocket);
}

/**
 * @brief Sends a custom Ping packet to a Server over UDP
 */
void Client::pingServer()
{
	Packet::Ping p;
	p.sendPacketUdp(&udpSocket, serverAddress);
}

/**
 * @brief Called, when a socket error occurred
 */
void Client::socketError(QAbstractSocket::SocketError)
{
	setJoinStatus(JoinStatus::FAILED);
	Gui::getSingleton().postInfoBar(tcpSocket.errorString());
}

/**
 * @brief Called, when the socket has connected
 */
void Client::socketConnected()
{
	// TCP connection successful, now try UDP
	setJoinStatus(JoinStatus::UDP_PENDING);
	pingServer();
}

/**
 * @brief Called, when the socket has disconnected
 */
void Client::socketDisconnected()
{
	setJoinStatus(JoinStatus::FAILED);
	Gui::getSingleton().postInfoBar("Disconnected");
}

/**
 * @brief Called, when there is new data available on the socket
 */
void Client::socketReadyRead()
{
	bool illformedPacket = false;
	while (tcpSocket.bytesAvailable() && !illformedPacket) {
		in.startTransaction();
		auto packet = Packet::AbstractPacket::receivePacket(in, InstanceType::Server);
		if (in.commitTransaction()) {
			handlePacket(packet);
		} else {
			qInfo() << "Received ill-formed packet";
			illformedPacket = true;
		}
	}
}

/**
 * @brief Handles an UDP socket error
 */
void Client::udpSocketError(QAbstractSocket::SocketError)
{
	qInfo() << udpSocket.errorString();
}

/**
 * @brief Handles incoming UDP datagrams
 */
void Client::udpSocketReadyRead()
{
	while (udpSocket.hasPendingDatagrams()) {
		// get datagram
		QByteArray datagram;
		datagram.resize(udpSocket.pendingDatagramSize());
		udpSocket.readDatagram(datagram.data(), datagram.size());
		QDataStream udpStream(&datagram, QIODevice::ReadOnly);
		udpStream.startTransaction();
		auto packet = Packet::AbstractPacket::receivePacket(udpStream, InstanceType::Server);
		if (udpStream.commitTransaction()) {
			handlePacket(packet);
		} else {
			qInfo() << "ill-formed udp packet";
		}
	}
}

/**
 * @brief Processes an already received packet
 * @param p The packet that was received
 */
void Client::handlePacket(std::unique_ptr<Packet::AbstractPacket> &p)
{
	// First handle flags
	if (p->start) {
		emit Gui::getSingleton().startGame();
	}
	if (p->reset) {
		emit resetRound();
	}
	switch (static_cast<Packet::ServerTypes>(p->type)) {
	case Packet::ServerTypes::Chat_Message:
	{
		auto *chatMsg = (Packet::ServerChatMsg *)p.get();
		ChatModel::getSingleton().appendMessage(chatMsg->username, chatMsg->message);
		break;
	}
	case Packet::ServerTypes::PlayerModelEdit:
	{
		auto *playerModel = (Packet::ServerPlayerModel *)p.get();
		playerModel->extract();
		break;
	}
	case Packet::ServerTypes::CurverData:
	{
		auto *curverData = (Packet::ServerCurverData *)p.get();
		curverData->extract();
		break;
	}
	case Packet::ServerTypes::ItemData:
	{
		auto *itemData = (Packet::ServerItemData *)p.get();
		emit integrateItem(itemData->spawned, itemData->sequenceNumber, itemData->which, itemData->pos, itemData->allowedUsers, itemData->collectorIndex);
		break;
	}
	case Packet::ServerTypes::SettingsType:
	{
		auto *settingsData = (Packet::ServerSettingsData *)p.get();
		settingsData->extract();
		break;
	}
	case Packet::ServerTypes::Pong:
	{
		auto *pong = (Packet::Pong *)p.get();
		qInfo() << Util::getTimeDiff(pong->sent);
		if (this->joinStatus == JoinStatus::UDP_PENDING) {
			setJoinStatus(JoinStatus::JOINED);
			sendPlayerModel();
		}
		break;
	}
	default:
		qInfo() << "Unsupported packet type";
		break;
	}
}

/**
 * @brief Sets the join status
 * @param s The new join status
 */
void Client::setJoinStatus(JoinStatus s)
{
	this->joinStatus = s;
	emit joinStatusChanged(s);
}
