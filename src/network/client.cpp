#include "client.h"

Client::Client()
{
	in.setDevice(&socket);
	connect(&socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
	connect(&socket, SIGNAL(connected()), this, SLOT(socketConnected()));
	connect(&socket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
	connect(&socket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
}

void Client::connectToHost(QHostAddress &addr, quint16 port)
{
	socket.connectToHost(addr, port);
}

void Client::sendChatMessage(QString msg)
{
	Packet::ClientChatMsg p;
	p.message = msg;
	p.sendPacket(&socket);
}

void Client::sendPlayerModel()
{
	Packet::ClientPlayerModel p;
	p.username = Settings::getSingleton().getClientName();
	p.color = Settings::getSingleton().getClientColor();
	p.sendPacket(&socket);
}

void Client::processKey(Qt::Key key, bool release)
{
	Packet::ClientCurverRotation p;
	if (release) {
		p.rotation = Curver::ROTATE_NONE;
	} else {
		// TODO: Allow custom keys
		if (key == Qt::Key_Left)
			p.rotation = Curver::ROTATE_LEFT;
		else
			p.rotation = Curver::ROTATE_RIGHT;
	}
	p.sendPacket(&socket);
}

void Client::socketError(QAbstractSocket::SocketError)
{
	Gui::getSingleton().postInfoBar(socket.errorString());
}

void Client::socketConnected()
{
	Gui::getSingleton().postInfoBar("Connected");
	emit connectedToServerChanged(true);
	sendPlayerModel();
}

void Client::socketDisconnected()
{
	Gui::getSingleton().postInfoBar("Disconnected");
	emit connectedToServerChanged(false);
}

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
