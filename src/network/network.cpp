#include "network.h"

Packet::AbstractPacket::AbstractPacket(Packet::PacketType type) : type(type)
{
}

Packet::AbstractPacket::~AbstractPacket()
{
}

void Packet::AbstractPacket::sendPacket(QTcpSocket *s) const
{
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	// write type and flags to stream
	uint8_t flags = 0;
	flags |= start << 5;
	flags |= reset << 4;
	uint8_t header = (type << 6) | flags;
	out << header;
	this->serialize(out);
	s->write(block);
}

std::unique_ptr<Packet::AbstractPacket> Packet::AbstractPacket::receivePacket(QDataStream &in, InstanceType from)
{
	std::unique_ptr<Packet::AbstractPacket> result;
	uint8_t header;
	in >> header;
	PacketType type = header >> 6;
	uint8_t flags = header << 2 >> 2;
	switch (from) {
	case InstanceType::Server:
		switch (static_cast<ServerTypes>(type)) {
		case ServerTypes::Chat_Message:
			result = std::make_unique<ServerChatMsg>();
			break;
		case ServerTypes::PlayerModelEdit:
			result = std::make_unique<ServerPlayerModel>();
			break;
		case ServerTypes::CurverData:
			result = std::make_unique<ServerCurverData>();
			break;
		case ServerTypes::ItemData:
			result = std::make_unique<ServerItemData>();
			break;
		default:
			qDebug() << "unsupported server package";
			in.rollbackTransaction();
		}
		break;
	case InstanceType::Client:
		switch (static_cast<ClientTypes>(type)) {
		case ClientTypes::Chat_Message:
			result = std::make_unique<ClientChatMsg>();
			break;
		case ClientTypes::PlayerModelEdit:
			result = std::make_unique<ClientPlayerModel>();
			break;
		case ClientTypes::CurverRotation:
			result = std::make_unique<ClientCurverRotation>();
			break;
		default:
			qDebug() << "unsupported client package";
			in.rollbackTransaction();
		}
		break;
	}
	if (result) {
		result->start = Util::getBit(flags, 5);
		result->reset = Util::getBit(flags, 4);
		result->parse(in);
	} else {
		qDebug() << "Received ill-formed package";
	}
	return result;
}

Packet::ServerChatMsg::ServerChatMsg() : AbstractPacket(static_cast<PacketType>(ServerTypes::Chat_Message))
{
}

void Packet::ServerChatMsg::serialize(QDataStream &out) const
{
	out << username << message;
}

void Packet::ServerChatMsg::parse(QDataStream &in)
{
	in >> username >> message;
}

Packet::ClientChatMsg::ClientChatMsg() : AbstractPacket(static_cast<PacketType>(ClientTypes::Chat_Message))
{
}

void Packet::ClientChatMsg::serialize(QDataStream &out) const
{
	out << message;
}

void Packet::ClientChatMsg::parse(QDataStream &in)
{
	in >> message;
}

QDataStream &Packet::operator <<(QDataStream &out, const Player &p)
{
	out << p.userName << p.color << p.roundScore << p.totalScore << static_cast<uint8_t>(p.controller);
	return out;
}

QDataStream &Packet::operator >>(QDataStream &in, Packet::Player &p)
{
	uint8_t ctrl;
	in >> p.userName >> p.color >> p.roundScore >> p.totalScore >> ctrl;
	p.controller = static_cast<Curver::Controller>(ctrl);
	return in;
}

Packet::ServerPlayerModel::ServerPlayerModel() : AbstractPacket(static_cast<PacketType>(ServerTypes::PlayerModelEdit))
{
}

void Packet::ServerPlayerModel::fill()
{
	QByteArray buf;
	QDataStream pipe(&buf, QIODevice::WriteOnly);
	PlayerModel::getSingleton().serialize(pipe);
	QDataStream in(&buf, QIODevice::ReadOnly);
	this->parse(in);
}

void Packet::ServerPlayerModel::extract()
{
	QByteArray buf;
	QDataStream pipe(&buf, QIODevice::WriteOnly);
	this->serialize(pipe);
	QDataStream in(&buf, QIODevice::ReadOnly);
	PlayerModel::getSingleton().parse(in);
}

void Packet::ServerPlayerModel::serialize(QDataStream &out) const
{
	Util::serializeCnt(out, data);
}

void Packet::ServerPlayerModel::parse(QDataStream &in)
{
	Util::parseCnt(in, data);
}


Packet::ClientPlayerModel::ClientPlayerModel() : AbstractPacket(static_cast<PacketType>(ClientTypes::PlayerModelEdit))
{
}

void Packet::ClientPlayerModel::serialize(QDataStream &out) const
{
	out << username << color;
}

void Packet::ClientPlayerModel::parse(QDataStream &in)
{
	in >> username >> color;
}

Packet::ServerCurverData::ServerCurverData() : AbstractPacket(static_cast<PacketType>(ServerTypes::CurverData))
{
}

void Packet::ServerCurverData::fill()
{
	auto &curvers = PlayerModel::getSingleton().getCurvers();
	pos.resize(curvers.size());
	changingSegment.resize(curvers.size());
	for (uint i = 0; i < curvers.size(); ++i) {
		pos[i] = curvers[i]->getPos();
		changingSegment[i] = curvers[i]->isChangingSegment();
	}
}

void Packet::ServerCurverData::extract()
{
	auto &curvers = PlayerModel::getSingleton().getCurvers();
	for (uint i = 0; i < curvers.size(); ++i) {
		curvers[i]->appendPoint(pos[i], changingSegment[i]);
	}
}

void Packet::ServerCurverData::serialize(QDataStream &out) const
{
	Util::serializeCnt(out, pos);
	// the following procedure serializes every changingSegment into a single bit
	uint8_t compressedByte = 0;
	int lastPos = 8;
	for (auto seg : changingSegment) {
		if (lastPos == 0) {
			out << compressedByte;
			compressedByte = 0;
			lastPos = 8;
		}
		compressedByte |= seg << --lastPos;
	}
	out << compressedByte;
}

void Packet::ServerCurverData::parse(QDataStream &in)
{
	Util::parseCnt(in, pos);
	// the following procedure parses every changingSegment bit by bit
	changingSegment.resize(pos.size());
	uint8_t compressedByte;
	int lastPos = 0;
	for (auto seg : changingSegment) {
		if (lastPos == 0) {
			in >> compressedByte;
			lastPos = 8;
		}
		seg = Util::getBit(compressedByte, --lastPos);
	}
}

Packet::ClientCurverRotation::ClientCurverRotation() : AbstractPacket(static_cast<PacketType>(ClientTypes::CurverRotation))
{
}

void Packet::ClientCurverRotation::serialize(QDataStream &out) const
{
	out << static_cast<uint8_t>(rotation);
}

void Packet::ClientCurverRotation::parse(QDataStream &in)
{
	uint8_t rot;
	in >> rot;
	rotation = static_cast<Curver::Rotation>(rot);
}

Packet::ServerItemData::ServerItemData() : AbstractPacket(static_cast<PacketType>(ServerTypes::ItemData))
{
}

void Packet::ServerItemData::serialize(QDataStream &out) const
{
	out << spawned << sequenceNumber << which << pos << static_cast<uint8_t>(allowedUsers) << collectorIndex;
}

void Packet::ServerItemData::parse(QDataStream &in)
{
	uint8_t allowed;
	in >> spawned >> sequenceNumber >> which >> pos >> allowed >> collectorIndex;
	allowedUsers = static_cast<Item::AllowedUsers>(allowed);
}
