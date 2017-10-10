#ifndef MULTIPLAYERSETTINGS_H
#define MULTIPLAYERSETTINGS_H

#include <QString>
#include <QtNetwork>

#include "gui.h"
#include "curver.h"
#include "models/playermodel.h"
#include "models/chatmodel.h"
#include "util.h"
#include "items/item.h"

enum class InstanceType : bool {
	Server,
	Client
};

namespace Packet {


using PacketType = uint8_t;

enum class ServerTypes : PacketType {
	Chat_Message,
	PlayerModelEdit,
	CurverData,
	ItemData
};

enum class ClientTypes : PacketType {
	Chat_Message,
	PlayerModelEdit,
	CurverRotation
};

class AbstractPacket
{
public:
	explicit AbstractPacket(PacketType type);
	virtual ~AbstractPacket();
	void sendPacket(QTcpSocket *s) const;
	static std::unique_ptr<AbstractPacket> receivePacket(QDataStream &in, InstanceType from);
	PacketType type;
	bool start = false;
	bool reset = false;
protected:
	virtual void serialize(QDataStream &out) const = 0;
	virtual void parse(QDataStream &in) = 0;
};

class ServerChatMsg : public AbstractPacket
{
public:
	ServerChatMsg();
	QString username;
	QString message;
protected:
	virtual void serialize(QDataStream &out) const override;
	virtual void parse(QDataStream &in) override;
};

class ClientChatMsg : public AbstractPacket
{
public:
	ClientChatMsg();
	QString message;
protected:
	virtual void serialize(QDataStream &out) const override;
	virtual void parse(QDataStream &in) override;
};

struct Player
{
	QString userName;
	QColor color;
	int roundScore;
	int totalScore;
	Curver::Controller controller;
};

QDataStream &operator <<(QDataStream &out, const Player &p);
QDataStream &operator >>(QDataStream &in, Player &p);

class ServerPlayerModel : public AbstractPacket
{
public:
	ServerPlayerModel();
	void fill();
	void extract();
	std::vector<Player> data;
protected:
	virtual void serialize(QDataStream &out) const override;
	virtual void parse(QDataStream &in) override;
};

class ClientPlayerModel : public AbstractPacket
{
public:
	ClientPlayerModel();
	QString username;
	QColor color;
protected:
	virtual void serialize(QDataStream &out) const override;
	virtual void parse(QDataStream &in) override;
};

class ServerCurverData : public AbstractPacket
{
public:
	ServerCurverData();
	void fill();
	void extract();
	std::vector<QPointF> pos;
	std::vector<bool> changingSegment;
protected:
	virtual void serialize(QDataStream &out) const override;
	virtual void parse(QDataStream &in) override;
};

class ClientCurverRotation : public AbstractPacket
{
public:
	ClientCurverRotation();
	Curver::Rotation rotation;
protected:
	virtual void serialize(QDataStream &out) const override;
	virtual void parse(QDataStream &in) override;
};

class ServerItemData : public AbstractPacket
{
public:
	ServerItemData();
	bool spawned = true;
	unsigned int sequenceNumber = 0;
	int which = 0;
	QPointF pos;
	Item::AllowedUsers allowedUsers = Item::AllowedUsers::ALLOW_ALL;
	int collectorIndex = -1;
protected:
	virtual void serialize(QDataStream &out) const override;
	virtual void parse(QDataStream &in) override;
};

}

#endif // MULTIPLAYERSETTINGS_H
