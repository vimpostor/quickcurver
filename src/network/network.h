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

/**
 * @brief An enumeration representing the instance type.
 *
 * One of Server and Client.
 */
enum class InstanceType : bool {
	Server,
	Client
};

/**
 * @brief The namespace containing all packets
 */
namespace Packet {

/**
 * @brief A data type representing the type of a packet
 */
using PacketType = uint8_t;
#define PACKET_TYPE_BITS 3

/**
 * @brief An enumeration containing all server packet types
 */
enum class ServerTypes : PacketType {
	Chat_Message,
	PlayerModelEdit,
	CurverData,
	ItemData,
	SettingsType,
};

/**
 * @brief An enumeration containing all client packet types
 */
enum class ClientTypes : PacketType {
	Chat_Message,
	PlayerModelEdit,
	CurverRotation,
};

/**
 * @brief A class representing an abstract packet.
 *
 * Contains all common packet members and their serialization patterns
 */
class AbstractPacket
{
public:
	explicit AbstractPacket(PacketType type);
	virtual ~AbstractPacket();
	void sendPacket(QTcpSocket *s) const;
	static std::unique_ptr<AbstractPacket> receivePacket(QDataStream &in, InstanceType from);
	/**
	 * @brief The packet type
	 */
	PacketType type;
	/**
	 * @brief Whether the game started
	 */
	bool start = false;
	/**
	 * @brief Whether the game resets
	 */
	bool reset = false;
protected:
	/**
	 * @brief Serializes a packet
	 * @param out The stream to serialize into
	 */
	virtual void serialize(QDataStream &out) const = 0;
	/**
	 * @brief Parses a packet from an incoming stream
	 * @param in The stream to parse from
	 */
	virtual void parse(QDataStream &in) = 0;
};

/**
 * @brief A packet that represents a chat message coming from a Server
 */
class ServerChatMsg : public AbstractPacket
{
public:
	ServerChatMsg();
	/**
	 * @brief The username of the sender
	 */
	QString username;
	/**
	 * @brief The chat message
	 */
	QString message;
protected:
	virtual void serialize(QDataStream &out) const override;
	virtual void parse(QDataStream &in) override;
};

/**
 * @brief A packet that represents a chat message coming from a Client
 */
class ClientChatMsg : public AbstractPacket
{
public:
	ClientChatMsg();
	/**
	 * @brief The chat message
	 */
	QString message;
protected:
	virtual void serialize(QDataStream &out) const override;
	virtual void parse(QDataStream &in) override;
};

/**
 * @brief A struct containing all Player data that is necessary to send over the network
 */
struct Player
{
	/**
	 * @brief The username
	 */
	QString userName;
	/**
	 * @brief The color
	 */
	QColor color;
	/**
	 * @brief The score in this round
	 */
	int roundScore;
	/**
	 * @brief The total score
	 */
	int totalScore;
	/**
	 * @brief The controller of the underlying Curver
	 */
	Curver::Controller controller;
	/**
	 * @brief Whether the player is alive
	 */
	bool isAlive;
};

QDataStream &operator <<(QDataStream &out, const Player &p);
QDataStream &operator >>(QDataStream &in, Player &p);

/**
 * @brief A packet that represents a PlayerModel change coming from a Server
 */
class ServerPlayerModel : public AbstractPacket
{
public:
	ServerPlayerModel();
	void fill();
	void extract();
	/**
	 * @brief A vector containing every Player data
	 */
	std::vector<Player> data;
protected:
	virtual void serialize(QDataStream &out) const override;
	virtual void parse(QDataStream &in) override;
};

/**
 * @brief A packet that represents a PlayerModel change coming from a Client
 */
class ClientPlayerModel : public AbstractPacket
{
public:
	ClientPlayerModel();
	/**
	 * @brief The username
	 */
	QString username;
	/**
	 * @brief The color
	 */
	QColor color;
protected:
	virtual void serialize(QDataStream &out) const override;
	virtual void parse(QDataStream &in) override;
};

/**
 * @brief A packet that represents a Curver data post from the Server
 */
class ServerCurverData : public AbstractPacket
{
public:
	ServerCurverData();
	void fill();
	void extract();
	/**
	 * @brief The new positions cumulated from every Curver
	 */
	std::vector<QPointF> pos;
	/**
	 * @brief Whether an individual Curver is changing segments at the moment
	 */
	std::vector<bool> changingSegment;
protected:
	virtual void serialize(QDataStream &out) const override;
	virtual void parse(QDataStream &in) override;
};

/**
 * @brief A packet that represents a Curver rotation change coming from the Client
 */
class ClientCurverRotation : public AbstractPacket
{
public:
	ClientCurverRotation();
	/**
	 * @brief The wanted rotation of the Curver
	 */
	Curver::Rotation rotation;
protected:
	virtual void serialize(QDataStream &out) const override;
	virtual void parse(QDataStream &in) override;
};

/**
 * @brief A packet that represents an Item event coming from a Server
 */
class ServerItemData : public AbstractPacket
{
public:
	ServerItemData();
	/**
	 * @brief Whether the Item was spawned or triggered
	 */
	bool spawned = true;
	/**
	 * @brief The sequence number uniquely identifying the Item
	 */
	unsigned int sequenceNumber = 0;
	/**
	 * @brief The kind of Item that spawned
	 */
	int which = 0;
	/**
	 * @brief The location of the Item
	 */
	QPointF pos;
	/**
	 * @brief The allowed users of the Item
	 */
	Item::AllowedUsers allowedUsers = Item::AllowedUsers::ALLOW_ALL;
	/**
	 * @brief If ServeritemData::spawned is false, this value represents the index of the collecting Curver
	 */
	int collectorIndex = -1;
protected:
	virtual void serialize(QDataStream &out) const override;
	virtual void parse(QDataStream &in) override;
};

/**
 * @brief A packet that represents game settings
 */
class ServerSettingsData : public AbstractPacket
{
public:
	ServerSettingsData();
	void fill();
	void extract();
	/**
	 * @brief The dimension of the game field
	 */
	QPoint dimension;
protected:
	virtual void serialize(QDataStream &out) const override;
	virtual void parse(QDataStream &in) override;
};

}

#endif // MULTIPLAYERSETTINGS_H
