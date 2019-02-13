#ifndef CLIENT_H
#define CLIENT_H
#include <QObject>
#include <QtNetwork>
#include <QTimer>

#include "network.h"

/**
 * @brief A Client network instance
 *
 * Handles the network communication from the client side
 */
class Client : public QObject
{
	Q_OBJECT

	Q_PROPERTY(JoinStatus joinStatus READ getJoinStatus NOTIFY joinStatusChanged)
public:
	/**
	 * @brief The current status of joining a Server
	 */
	enum class JoinStatus {
		NONE,
		FAILED,
		DNS_PENDING,
		TCP_PENDING,
		UDP_PENDING,
		JOINED
	};
	Q_ENUM(JoinStatus)

	Client();

	Q_INVOKABLE JoinStatus getJoinStatus() const;

	void connectToHost(QString addr, quint16 port);
	void sendChatMessage(QString msg);
	void sendPlayerModel();
	void processKey(Qt::Key key, bool release);
	void pingServer();
signals:
	/**
	 * @brief Emitted when a new Item has to be integrated into the Game
	 * @param spawned Whether the Item spawned or was triggered
	 * @param sequenceNumber The unique sequence number of the Item
	 * @param which The kind of Item
	 * @param pos The location of the Item
	 * @param allowedUsers The allowed users for the Item
	 * @param collectorIndex If \a spawned is \c false, then this indicates the collecting Curver
	 */
	void integrateItem(bool spawned, unsigned int sequenceNumber, int which, QPointF pos, Item::AllowedUsers allowedUsers, int collectorIndex);
	/**
	 * @brief Emitted when the round has to be reset
	 */
	void resetRound();
	/**
	 * @brief Emitted when the join status changed
	 * @param joinStatus The new join status
	 */
	void joinStatusChanged(JoinStatus joinStatus);
private slots:
	// tcp
	void socketError(QAbstractSocket::SocketError);
	void socketConnected();
	void socketDisconnected();
	void socketReadyRead();
	// udp
	void udpSocketError(QAbstractSocket::SocketError);
	void udpSocketReadyRead();
private:
	void handlePacket(std::unique_ptr<Packet::AbstractPacket> &p);
	void setJoinStatus(JoinStatus s);
	/**
	 * @brief The TCP socket to communicate with
	 */
	QTcpSocket tcpSocket;
	/**
	 * @brief The UDP socket to communicate with
	 */
	QUdpSocket udpSocket;
	/**
	 * @brief A data stream belonging to Client::socket
	 */
	QDataStream in;
	/**
	 * @brief The address of the server to connect to
	 */
	FullNetworkAddress serverAddress;
	/**
	 * @brief The current join status
	 */
	JoinStatus joinStatus = JoinStatus::NONE;
	/**
	 * @brief The timer responsible for continuously sending a Ping to the Server
	 */
	QTimer pingTimer;
};

#endif // CLIENT_H
