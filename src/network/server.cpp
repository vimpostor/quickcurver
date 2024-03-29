#include "server.hpp"

Server::Server() {
	connect(&tcpServer, &QTcpServer::acceptError, this, &Server::acceptError);
	connect(&tcpServer, &QTcpServer::newConnection, this, &Server::newConnection);
	connect(Settings::get(), &Settings::dimensionChanged, this, &Server::broadcastSettings);
	connect(&udpSocket, &QUdpSocket::errorOccurred, this, &Server::udpSocketError);
	connect(&udpSocket, &QUdpSocket::readyRead, this, &Server::udpSocketReadyRead);
	reListen(0);
}

Server::~Server() {
	clients.clear();
}

/**
 * @brief Broadcasts new Curver data to every Client
 */
void Server::broadcastCurverData() {
	if (++dataBroadcastIteration % Settings::get()->getNetworkCurverBlock() == 0) {
		Packet::ServerCurverData p;
		p.fill();
		p.start = true;
		// if reset is due, send reset and reset the reset flag
		p.reset = resetDue;
		resetDue = false;
		broadcastPacket(p, true);
	}
}

/**
 * @brief Broadcasts a chat message to every Client
 * @param username The author of the message
 * @param message The chat message
 */
void Server::broadcastChatMessage(QString username, QString message) {
	Packet::ServerChatMsg p;
	p.username = username;
	p.message = message;
	ChatModel::get()->appendMessage(username, message);
	broadcastPacket(p);
}

/**
 * @brief Broadcasts an admin chat message to every Client
 * @param msg The chat message to broadcast
 */
void Server::broadcastChatMessage(QString msg) {
	broadcastChatMessage(ADMIN_NAME, msg);
}

/**
 * @brief Broadcasts the game settings to every Client
 */
void Server::broadcastSettings() {
	Packet::ServerSettingsData p;
	p.fill();
	broadcastPacket(p);
}

/**
 * @brief Resets the current round
 */
void Server::resetRound() {
	resetDue = true;
}

/**
 * @brief Reconfigures the Server to listen on another port
 * @param port The new port to listen on
 */
void Server::reListen(quint16 port) {
	tcpServer.close();
	tcpServer.listen(QHostAddress::Any, port);
	udpSocket.close();
	udpSocket.bind(tcpServer.serverPort());
	qDebug() << "Running on port" << tcpServer.serverPort();
}

/**
 * @brief Broadcasts the PlayerModel to every Client
 */
void Server::broadcastPlayerModel() {
	Packet::ServerPlayerModel p;
	p.fill();
	broadcastPacket(p);
}

/**
 * @brief Broadcasts a new Item event to every Client
 * @param spawned Whether the Item spawned or was triggered
 * @param sequenceNumber The unique sequence number of the Item
 * @param which The kind of Item
 * @param pos The location of the Item
 * @param allowedUsers The allowed users for the Item
 * @param collectorIndex If \a spawned is \c false, this value defines which Curver collected the Item
 */
void Server::broadcastItemData(bool spawned, unsigned int sequenceNumber, int which, QPointF pos, Item::AllowedUsers allowedUsers, int collectorIndex) {
	Packet::ServerItemData p;
	p.spawned = spawned;
	p.sequenceNumber = sequenceNumber;
	p.which = which;
	p.pos = pos;
	p.allowedUsers = allowedUsers;
	p.collectorIndex = collectorIndex;
	broadcastPacket(p);
}

/**
 * @brief This function is called, when an error occurred during accepting an incoming connection
 */
void Server::acceptError(QAbstractSocket::SocketError) {
	qDebug() << tcpServer.errorString();
}

/**
 * @brief This function is called when there is a new connection pending
 */
void Server::newConnection() {
	QTcpSocket *s = tcpServer.nextPendingConnection();
	// socket must not be NULL
	if (s) {
		auto *curver = PlayerModel::get()->getNewPlayer();
		curver->controller = Curver::Controller::CONTROLLER_REMOTE;
		clients[s] = curver;
		connect(s, &QTcpSocket::errorOccurred, this, &Server::socketError);
		connect(s, &QTcpSocket::disconnected, this, &Server::socketDisconnect);
		connect(s, &QTcpSocket::readyRead, this, &Server::socketReadyRead);

		// the username is not sent yet, so we cannot pretty print the name yet
		broadcastChatMessage(s->peerAddress().toString() + " joined");
		broadcastSettings();
	}
}

/**
 * @brief This function is called, when there was a socket error
 */
void Server::socketError(QAbstractSocket::SocketError) {
	QTcpSocket *s = static_cast<QTcpSocket *>(sender());
	qDebug() << s->errorString();
	removePlayer(s);
}

/**
 * @brief This function is called, when a socket disconnected
 */
void Server::socketDisconnect() {
	QTcpSocket *s = static_cast<QTcpSocket *>(sender());
	removePlayer(s);
}

/**
 * @brief This function is called, when there is data available to read from a socket
 */
void Server::socketReadyRead() {
	QTcpSocket *s = static_cast<QTcpSocket *>(sender());
	QDataStream in(s);
	bool illformedPacket = false;
	while (s->bytesAvailable() && !illformedPacket) {
		in.startTransaction();
		auto packet = Packet::AbstractPacket::receivePacket(in, InstanceType::Client);
		if (in.commitTransaction()) {
			handlePacket(packet, s);
		} else {
			qDebug() << "received ill-formed packet";
			illformedPacket = true;
		}
	}
}

/**
 * @brief Handles an UDP socket error
 */
void Server::udpSocketError(QAbstractSocket::SocketError) {
	qDebug() << "UDP error" << udpSocket.errorString();
}

/**
 * @brief Handles incoming UDP packets
 */
void Server::udpSocketReadyRead() {
	while (udpSocket.hasPendingDatagrams()) {
		// get datagram
		QByteArray datagram;
		datagram.resize(udpSocket.pendingDatagramSize());
		QHostAddress sender;
		quint16 port;
		udpSocket.readDatagram(datagram.data(), datagram.size(), &sender, &port);
		FullNetworkAddress client = {sender, port};
		QDataStream udpStream(&datagram, QIODevice::ReadOnly);
		udpStream.startTransaction();
		auto packet = Packet::AbstractPacket::receivePacket(udpStream, InstanceType::Client);
		if (udpStream.commitTransaction()) {
			handlePacket(packet, nullptr, client);
			// subscribe client to updates if not yet subscribed
			if (std::ranges::find(udpAddresses, client) == udpAddresses.end()) {
				udpAddresses.push_back(client);
			}
		} else {
			qInfo() << "Got an ill-formed UDP packet";
		}
	}
}

/**
 * @brief Removes a player permanently
 * @param s The socket that defines the Curver to remove
 */
void Server::removePlayer(const QTcpSocket *s) {
	auto it = std::ranges::find_if(clients, [=](const auto &c) { return c.first == s; });
	if (it != clients.end()) {
		auto *c = it->second;
		// remove from TCP list (Qt will delete the socket later, when the server shuts down)
		clients.erase(it);
		// remove from UDP list
		auto udpit = std::ranges::find_if(udpAddresses, [&](const auto &a) { return a.addr == s->peerAddress(); });
		if (udpit != udpAddresses.cend()) {
			udpAddresses.erase(udpit);
		}
		// remove from player model
		PlayerModel::get()->removeCurver(c);
		broadcastChatMessage(curverNetworkName(s, c) + " left the game");
	}
}

/**
 * @brief Processes an already received packet
 * @param p The packet to process
 * @param s The socket that the packet was received with
 * @param sender The sender of the packet, if sent via UDP
 */
void Server::handlePacket(std::unique_ptr<Packet::AbstractPacket> &p, const QTcpSocket *s, FullNetworkAddress sender) {
	Curver *curver = nullptr;
	if (s != nullptr) {
		curver = curverFromSocket(s);
	}
	// packet types that require curver to be set
	const std::array<Packet::ClientTypes, 3> needsCurver = {
		Packet::ClientTypes::Chat_Message,
		Packet::ClientTypes::PlayerModelEdit,
		Packet::ClientTypes::CurverRotation,
	};
	const auto packetType = static_cast<Packet::ClientTypes>(p->type);
	if (curver == nullptr && std::ranges::find(needsCurver, packetType) != needsCurver.end()) {
		qDebug() << "curver is not set";
		return;
	}
	// TODO: Deal with flags
	switch (packetType) {
	case Packet::ClientTypes::Chat_Message:
		{
			QString msg = ((Packet::ClientChatMsg *) p.get())->message;
			broadcastChatMessage(curver->userName, msg);
			break;
		}
	case Packet::ClientTypes::PlayerModelEdit:
		{
			auto *playerData = (Packet::ClientPlayerModel *) p.get();
			curver->userName = playerData->username;
			curver->setColor(playerData->color);
			PlayerModel::get()->forceRefresh();
			break;
		}
	case Packet::ClientTypes::CurverRotation:
		{
			if (curver) {
				curver->rotation = ((Packet::ClientCurverRotation *) p.get())->rotation;
			}
			break;
		}
	case Packet::ClientTypes::Ping:
		{
			auto *pingPacket = (Packet::Ping *) p.get();
			// respond with pong
			Packet::Pong pongPacket;
			pongPacket.sent = pingPacket->sent;
			pongPacket.curverIndex = getCurverIndex(sender);

			if (pongPacket.curverIndex != -1) {
				// store current ping of this player
				PlayerModel::get()->getCurvers()[pongPacket.curverIndex]->ping = pingPacket->delta;
				PlayerModel::get()->forceRefresh();
			}

			pongPacket.fill();
			pongPacket.sendPacketUdp(&udpSocket, sender);
			break;
		}
	default:
		qDebug() << "Unsupported packet type";
		break;
	}
}

/**
 * @brief Broadcasts a packet to every Client
 * @param p The packet to broadcast
 * @param udp Whether to broadcast using UDP or TCP
 */
void Server::broadcastPacket(Packet::AbstractPacket &p, bool udp) {
	if (udp) {
		std::ranges::for_each(udpAddresses, [&](auto &c) { p.sendPacketUdp(&udpSocket, c); });
	} else {
		std::ranges::for_each(clients, [&](auto &c) { p.sendPacket(c.first); });
	}
}

/**
 * @brief Computes the index belonging to a client
 * @param peer The address of the client
 * @return The index in the server-side curver array
 */
int Server::getCurverIndex(const FullNetworkAddress peer) {
	auto it = std::ranges::find_if(clients, [&](const auto &p) { return p.first->peerAddress() == peer.addr; });
	if (it != clients.cend()) {
		auto &curvers = PlayerModel::get()->getCurvers();
		for (size_t i = 0; i < curvers.size(); ++i) {
			if (curvers[i].get() == it->second) {
				return i;
			}
		}
	}
	qDebug() << "Client index not found" << peer.addr << peer.port;
	return -1;
}

/**
 * @brief Returns a pretty-printed name for a network user
 * @param s The socket corresponding to the user
 * @param curver The curver corresponding to the user
 * @return A pretty-printed name
 */
QString Server::curverNetworkName(const QTcpSocket *s, const Curver *curver) {
	return QString("%1 (%2)").arg(s->peerAddress().toString()).arg(curver->userName);
}

/**
 * @brief Returns the Curver connected to a given socket
 * @param s The socket to return the Curver of
 * @return The Curver that belongs to \a s
 */
Curver *Server::curverFromSocket(const QTcpSocket *s) const {
	auto it = std::ranges::find_if(clients, [&](const auto &c) { return c.first == s; });
	if (it != clients.cend()) {
		return it->second;
	} else {
		return nullptr;
	}
}
