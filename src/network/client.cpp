#include "client.h"

Client::Client()
{
	in.setDevice(&socket);
	connect(&socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &Client::socketError);
	connect(&socket, &QTcpSocket::connected, this, &Client::socketConnected);
	connect(&socket, &QTcpSocket::disconnected, this, &Client::socketDisconnected);
	connect(&socket, &QTcpSocket::readyRead, this, &Client::socketReadyRead);
}

/**
 * @brief Connects the Client to the given host
 * @param addr The IP address of the host to connect to
 * @param port The port that the host is listening on
 */
void Client::connectToHost(QHostAddress &addr, quint16 port)
{
	socket.connectToHost(addr, port);
}

/**
 * @brief Sends a chat message for the Server to broadcast
 * @param msg The chat message
 */
void Client::sendChatMessage(QString msg)
{
	Packet::ClientChatMsg p;
	p.message = msg;
	p.sendPacket(&socket);
}

/**
 * @brief Sends the PlayerModel of the Client to the Server
 */
void Client::sendPlayerModel()
{
	Packet::ClientPlayerModel p;
	p.username = Settings::getSingleton().getClientName();
	p.color = Settings::getSingleton().getClientColor();
	p.sendPacket(&socket);
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
	p.sendPacket(&socket);
}

/**
 * @brief Called, when a socket error occurred
 */
void Client::socketError(QAbstractSocket::SocketError)
{
	Gui::getSingleton().postInfoBar(socket.errorString());
}

/**
 * @brief Called, when the socket has connected
 */
void Client::socketConnected()
{
	Gui::getSingleton().postInfoBar("Connected");
	emit connectedToServerChanged(true);
	sendPlayerModel();
}

/**
 * @brief Called, when the socket has disconnected
 */
void Client::socketDisconnected()
{
	Gui::getSingleton().postInfoBar("Disconnected");
	emit connectedToServerChanged(false);
}

/**
 * @brief Called, when there is new data available on the socket
 */
void Client::socketReadyRead()
{
	bool illformedPacket = false;
	while (socket.bytesAvailable() && !illformedPacket) {
		in.startTransaction();
		auto packet = Packet::AbstractPacket::receivePacket(in, InstanceType::Server);
		if (in.commitTransaction()) {
			handlePacket(packet);
		} else {
			qDebug() << "Received ill-formed packet";
			illformedPacket = true;
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
	default:
		qDebug() << "unsupported package type";
		break;
	}
}
