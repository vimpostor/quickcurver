#include "server.h"

Server::Server()
{
	connect(&tcpServer, SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(acceptError(QAbstractSocket::SocketError)));
	connect(&tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
	reListen(0);
}

Server::~Server()
{
	clients.clear();
}

void Server::broadcastCurverData()
{
	Packet::ServerCurverData p;
	p.fill();
	p.start = true;
	// if reset is due, send reset and reset the reset flag
	p.reset = resetDue;
	resetDue = false;
	broadcastPacket(p);
}

void Server::broadcastChatMessage(QString username, QString message)
{
	Packet::ServerChatMsg p;
	p.username = username;
	p.message = message;
	ChatModel::getSingleton().appendMessage(username, message);
	broadcastPacket(p);
}

void Server::broadcastChatMessage(QString msg)
{
	broadcastChatMessage(ADMIN_NAME, msg);
}

void Server::resetRound()
{
	resetDue = true;
}

void Server::reListen(quint16 port)
{
	tcpServer.close();
	tcpServer.listen(QHostAddress::Any, port);
	qDebug() << "Running on port " << tcpServer.serverPort();
}

void Server::broadcastPlayerModel()
{
	Packet::ServerPlayerModel p;
	p.fill();
	broadcastPacket(p);
}

void Server::broadcastItemData(bool spawned, unsigned int sequenceNumber, int which, QPointF pos, Item::AllowedUsers allowedUsers, int collectorIndex)
{
	Packet::ServerItemData p;
	p.spawned = spawned;
	p.sequenceNumber = sequenceNumber;
	p.which = which;
	p.pos = pos;
	p.allowedUsers = allowedUsers;
	p.collectorIndex = collectorIndex;
	broadcastPacket(p);
}

void Server::acceptError(QAbstractSocket::SocketError)
{
	Gui::getSingleton().postInfoBar(tcpServer.errorString());
}

void Server::newConnection()
{
	QTcpSocket *s = tcpServer.nextPendingConnection();
	// socket must not be NULL
	if (s) {
		auto *curver = PlayerModel::getSingleton().getNewPlayer();
		curver->controller = Curver::Controller::CONTROLLER_REMOTE;
		clients[std::unique_ptr<QTcpSocket>(s)] = curver;
		connect(s, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
		connect(s, SIGNAL(disconnected()), this, SLOT(socketDisconnect()));
		connect(s, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
		broadcastChatMessage(s->peerAddress().toString() + " joined");
	}
}

void Server::socketError(QAbstractSocket::SocketError)
{
	QTcpSocket *s = static_cast<QTcpSocket *>(sender());
	Gui::getSingleton().postInfoBar(s->errorString());
	removePlayer(s);
}

void Server::socketDisconnect()
{
	QTcpSocket *s = static_cast<QTcpSocket *>(sender());
	removePlayer(s);
}

void Server::socketReadyRead()
{
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

void Server::removePlayer(const QTcpSocket *s)
{
	// TODO: Reconsider, whether Server should remove the Curver from the player model as well
	// TODO: Actually delete something here: Note, deleting here results in a segfault, because after this method was called in socketError()
	// or in socketDisconnect(), the TcpSocket seems to be still in use until after those methods are completely finished.

//	auto it = Util::find_if(clients, [=](const auto &c){ return c.first.get() == s; });
//	if (it != clients.end()) {
//		clients.erase(it);
	//	}
	broadcastChatMessage(s->peerAddress().toString() + " left the game");
}

void Server::handlePacket(std::unique_ptr<Packet::AbstractPacket> &p, const QTcpSocket *s)
{
	Curver *curver = curverFromSocket(s);
	// TODO: Deal with flags
	switch (static_cast<Packet::ClientTypes>(p->type)) {
	case Packet::ClientTypes::Chat_Message:
	{
		QString msg = ((Packet::ClientChatMsg *)p.get())->message;
		broadcastChatMessage(curver->userName, msg);
		break;
	}
	case Packet::ClientTypes::PlayerModelEdit:
	{
		auto *playerData = (Packet::ClientPlayerModel *)p.get();
		curver->userName = playerData->username;
		curver->setColor(playerData->color);
		PlayerModel::getSingleton().forceRefresh();
		break;
	}
	case Packet::ClientTypes::CurverRotation:
	{
		if (curver) {
			curver->rotation = ((Packet::ClientCurverRotation *)p.get())->rotation;
		}
		break;
	}
	default:
		qDebug() << "Unsupported package type";
		break;
	}
}

void Server::broadcastPacket(Packet::AbstractPacket &p)
{
	Util::for_each(clients, [&](auto &c){ p.sendPacket(c.first.get()); });
}

Curver *Server::curverFromSocket(const QTcpSocket *s) const
{
	auto it = Util::find_if(clients, [&](auto &c){ return c.first.get() == s; });
	if (it != clients.end()) {
		return it->second;
	} else {
		return nullptr;
	}
}
