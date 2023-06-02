#include "network.hpp"

bool operator==(const FullNetworkAddress &l, const FullNetworkAddress &r) {
	return l.addr == r.addr && l.port == r.port;
}

/**
 * @brief Constructs an AbstractPacket with the given type
 * @param type The type of the AbstractPacket
 */
Packet::AbstractPacket::AbstractPacket(Packet::PacketType type)
	: type(type) {
}

Packet::AbstractPacket::~AbstractPacket() {
}

/**
 * @brief Sends the packet over the network via TCP
 *
 * This serializes the data using the overloaded AbstractPacket::serialize() function
 * @param s The socket to send with
 */
void Packet::AbstractPacket::sendPacket(QTcpSocket *s) const {
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	// write type and flags to stream
	uint8_t header = 0;
	header |= type << (8 - PACKET_TYPE_BITS);
	Util::setBit(header, 0, start);
	Util::setBit(header, 1, reset);
	out << header;
	this->serialize(out);
	s->write(block);
}

/**
 * @brief Sends the packet via UDP
 * @param s The socket to send with
 * @param a The address to send to
 */
void Packet::AbstractPacket::sendPacketUdp(QUdpSocket *s, FullNetworkAddress a) const {
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	// write type and flags to stream
	uint8_t header = 0;
	header |= type << (8 - PACKET_TYPE_BITS);
	Util::setBit(header, 0, start);
	Util::setBit(header, 1, reset);
	out << header;
	this->serialize(out);
	s->writeDatagram(block, a.addr, a.port);
}

/**
 * @brief Receives a packet from a data stream
 *
 * The packet type is automatically deducted.
 * @param in The data stream to parse a packet from
 * @param from Whether the sender was a Server or Client instance
 * @return The received packet. If parsing was not successful, the smart pointer holds a nullptr
 */
std::unique_ptr<Packet::AbstractPacket> Packet::AbstractPacket::receivePacket(QDataStream &in, InstanceType from) {
	std::unique_ptr<Packet::AbstractPacket> result;
	uint8_t header;
	in >> header;
	PacketType type = header >> (8 - PACKET_TYPE_BITS);
	uint8_t flags = header << PACKET_TYPE_BITS >> PACKET_TYPE_BITS;
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
		case ServerTypes::SettingsType:
			result = std::make_unique<ServerSettingsData>();
			break;
		case ServerTypes::Pong:
			result = std::make_unique<Pong>();
			break;
		default:
			qDebug() << "unsupported server packet";
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
		case ClientTypes::Ping:
			result = std::make_unique<Ping>();
			break;
		default:
			qDebug() << "unsupported client packet";
			in.rollbackTransaction();
		}
		break;
	}
	if (result) {
		result->start = Util::getBit(flags, 0);
		result->reset = Util::getBit(flags, 1);
		result->parse(in);
	} else {
		qDebug() << "Received ill-formed packet";
	}
	return result;
}

/**
 * @brief Constructs a ServerChatMsg packet
 */
Packet::ServerChatMsg::ServerChatMsg()
	: AbstractPacket(static_cast<PacketType>(ServerTypes::Chat_Message)) {
}

/**
 * @brief Serializes the ServerChatMsg
 * @param out The stream to serialize into
 */
void Packet::ServerChatMsg::serialize(QDataStream &out) const {
	out << username << message;
}

/**
 * @brief Parses a ServerChatMsg
 * @param in The stream to parse from
 */
void Packet::ServerChatMsg::parse(QDataStream &in) {
	in >> username >> message;
}

/**
 * @brief Constructs a ClientChatMsg
 */
Packet::ClientChatMsg::ClientChatMsg()
	: AbstractPacket(static_cast<PacketType>(ClientTypes::Chat_Message)) {
}

/**
 * @brief Serializes the ClientChatMsg
 * @param out The stream to serialize into
 */
void Packet::ClientChatMsg::serialize(QDataStream &out) const {
	out << message;
}

/**
 * @brief Parses a ClientChatMsg
 * @param in The stream to parse from
 */
void Packet::ClientChatMsg::parse(QDataStream &in) {
	in >> message;
}

/**
 * @brief Serializes a Player
 * @param out The stream to serialize into
 * @param p The Player to serialize
 * @return \a out
 */
QDataStream &Packet::operator<<(QDataStream &out, const Player &p) {
	out << p.userName << p.color << p.roundScore << p.totalScore << static_cast<uint8_t>(p.controller) << static_cast<uint8_t>(p.isAlive);
	return out;
}

/**
 * @brief Parses a Player from a stream
 * @param in The stream to parse from
 * @param p The Player to parse
 * @return \a in
 */
QDataStream &Packet::operator>>(QDataStream &in, Packet::Player &p) {
	uint8_t ctrl, isAlive;
	in >> p.userName >> p.color >> p.roundScore >> p.totalScore >> ctrl >> isAlive;
	p.controller = static_cast<Curver::Controller>(ctrl);
	p.isAlive = static_cast<bool>(isAlive);
	return in;
}

/**
 * @brief  Constructs a ServerPlayerModel
 */
Packet::ServerPlayerModel::ServerPlayerModel()
	: AbstractPacket(static_cast<PacketType>(ServerTypes::PlayerModelEdit)) {
}

/**
 * @brief Automatically fills the packet with the according data
 */
void Packet::ServerPlayerModel::fill() {
	QByteArray buf;
	QDataStream pipe(&buf, QIODevice::WriteOnly);
	PlayerModel::getSingleton().serialize(pipe);
	QDataStream in(&buf, QIODevice::ReadOnly);
	this->parse(in);
}

/**
 * @brief Automatically extracts the packet data
 */
void Packet::ServerPlayerModel::extract() {
	QByteArray buf;
	QDataStream pipe(&buf, QIODevice::WriteOnly);
	this->serialize(pipe);
	QDataStream in(&buf, QIODevice::ReadOnly);
	PlayerModel::getSingleton().parse(in);
}

/**
 * @brief Serializes the ServerPlayerModel
 * @param out The stream to serialize into
 */
void Packet::ServerPlayerModel::serialize(QDataStream &out) const {
	Util::serializeCnt(out, data);
}

/**
 * @brief Parses a ClientChatMsg
 * @param in The stream to parse from
 */
void Packet::ServerPlayerModel::parse(QDataStream &in) {
	Util::parseCnt(in, data);
}

/**
 * @brief Constructs a ClientPlayerModel
 */
Packet::ClientPlayerModel::ClientPlayerModel()
	: AbstractPacket(static_cast<PacketType>(ClientTypes::PlayerModelEdit)) {
}

/**
 * @brief Serializes the ClientPlayerModel
 * @param out The stream to serialize into
 */
void Packet::ClientPlayerModel::serialize(QDataStream &out) const {
	out << username << color;
}

/**
 * @brief Parses a ClientPlayerModel
 * @param in The stream to parse from
 */
void Packet::ClientPlayerModel::parse(QDataStream &in) {
	in >> username >> color;
}

/**
 * @brief Constructs a ServerCurverData
 */
Packet::ServerCurverData::ServerCurverData()
	: AbstractPacket(static_cast<PacketType>(ServerTypes::CurverData)) {
}

/**
 * @brief Automatically fills the packet with the according data
 */
void Packet::ServerCurverData::fill() {
	auto &curvers = PlayerModel::getSingleton().getCurvers();
	pos.resize(curvers.size());
	changingSegment.resize(curvers.size());
	for (uint i = 0; i < curvers.size(); ++i) {
		pos[i] = curvers[i]->getPos();
		changingSegment[i] = curvers[i]->isChangingSegment();
	}
}

/**
 * @brief Automatically extracts the packet data
 */
void Packet::ServerCurverData::extract() {
	auto &curvers = PlayerModel::getSingleton().getCurvers();
	if (curvers.size() != pos.size()) {
		// invalid size, can be UDP related
		return;
	}
	for (uint i = 0; i < curvers.size(); ++i) {
		curvers[i]->appendPoint(pos[i], changingSegment[i]);
	}
}

/**
 * @brief Serializes the ServerCurverData
 * @param out The stream to serialize into
 */
void Packet::ServerCurverData::serialize(QDataStream &out) const {
	Util::serializeCnt(out, pos);
	// the following procedure serializes every changingSegment into a single byte
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

/**
 * @brief Parses a ServerCurverData
 * @param in The stream to parse from
 */
void Packet::ServerCurverData::parse(QDataStream &in) {
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

/**
 * @brief Constructs a ClientCurverRotation
 */
Packet::ClientCurverRotation::ClientCurverRotation()
	: AbstractPacket(static_cast<PacketType>(ClientTypes::CurverRotation)) {
}

/**
 * @brief Serializes the ClientCurverRotation
 * @param out The stream to serialize into
 */
void Packet::ClientCurverRotation::serialize(QDataStream &out) const {
	out << static_cast<uint8_t>(rotation);
}

/**
 * @brief Parses a ClientCurverRotation
 * @param in The stream to parse from
 */
void Packet::ClientCurverRotation::parse(QDataStream &in) {
	uint8_t rot;
	in >> rot;
	rotation = static_cast<Curver::Rotation>(rot);
}

/**
 * @brief Constructs a ServerItemData
 */
Packet::ServerItemData::ServerItemData()
	: AbstractPacket(static_cast<PacketType>(ServerTypes::ItemData)) {
}

/**
 * @brief Serializes the ServerItemData
 * @param out The stream to serialize into
 */
void Packet::ServerItemData::serialize(QDataStream &out) const {
	out << spawned << sequenceNumber << which << pos << static_cast<uint8_t>(allowedUsers) << collectorIndex;
}

/**
 * @brief Parses a ServerItemData
 * @param in The stream to parse from
 */
void Packet::ServerItemData::parse(QDataStream &in) {
	uint8_t allowed;
	in >> spawned >> sequenceNumber >> which >> pos >> allowed >> collectorIndex;
	allowedUsers = static_cast<Item::AllowedUsers>(allowed);
}

/**
 * @brief Constructs a Ping
 */
Packet::Ping::Ping()
	: AbstractPacket(static_cast<PacketType>(ClientTypes::Ping)) {
}

/**
 * @brief Serializes the Ping
 * @param out The stream to serialize into
 */
void Packet::Ping::serialize(QDataStream &out) const {
	out << sent << delta;
}

/**
 * @brief Parses a Ping
 * @param in The stream to parse from
 */
void Packet::Ping::parse(QDataStream &in) {
	in >> sent >> delta;
}

/**
 * @brief Constructs a ServerSettingsData
 */
Packet::ServerSettingsData::ServerSettingsData()
	: AbstractPacket(static_cast<PacketType>(ServerTypes::SettingsType)) {
}

/**
 * @brief Automatically fills the packet
 */
void Packet::ServerSettingsData::fill() {
	this->dimension = Settings::getSingleton().getDimension();
}

/**
 * @brief Automatically extracts the packet
 */
void Packet::ServerSettingsData::extract() {
	Settings::getSingleton().setDimension(this->dimension);
}

/**
 * @brief Serializes the ServerSettingsData
 * @param out The stream to serialize into
 */
void Packet::ServerSettingsData::serialize(QDataStream &out) const {
	out << dimension;
}

/**
 * @brief Parses a ServerSettingsData
 * @param in The stream to parse from
 */
void Packet::ServerSettingsData::parse(QDataStream &in) {
	in >> dimension;
}

/**
 * @brief Constructs a Pong
 *
 * This is an answer packet to Ping
 */
Packet::Pong::Pong()
	: AbstractPacket(static_cast<PacketType>(ServerTypes::Pong)) {
}

/**
 * @brief Automatically fills the packet with the according data
 */
void Packet::Pong::fill() {
	auto &curvers = PlayerModel::getSingleton().getCurvers();
	for (auto &c : curvers) {
		pings.push_back(c->ping);
	}
}

/**
 * @brief Automatically extracts the packet data
 */
void Packet::Pong::extract() {
	auto &curvers = PlayerModel::getSingleton().getCurvers();
	for (size_t i = 0; i < std::min(curvers.size(), pings.size()); ++i) {
		curvers[i]->ping = pings[i];
	}
}

/**
 * @brief Serializes a Pong
 * @param out The stream to serialize into
 */
void Packet::Pong::serialize(QDataStream &out) const {
	out << sent << curverIndex << static_cast<qint64>(pings.size());
	for (auto p : pings) {
		out << p;
	}
}

/**
 * @brief Parses a Pong
 * @param in The stream to parse from
 */
void Packet::Pong::parse(QDataStream &in) {
	qint64 size;
	qint64 ping;

	in >> sent >> curverIndex >> size;
	for (auto i = 0; i < size; ++i) {
		in >> ping;
		pings.push_back(ping);
	}
}
