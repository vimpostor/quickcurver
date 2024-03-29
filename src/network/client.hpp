#pragma once

#include <QObject>
#include <QTimer>
#include <QtNetwork>

#include "network.hpp"

/**
 * @brief A Client network instance
 *
 * Handles the network communication from the client side
 */
class Client : public QObject {
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
	void joinStatusChanged(const JoinStatus joinStatus);
	/**
	 * @brief Emitted when the DNS replied
	 * @param info The DNS reply
	 */
	void dnsFinished(QHostInfo info);
	/**
	 * @brief Emitted when the graphics stack needs to push a new frame
	 *
	 * This can happen for example when the \a Client receives new \a Curver data.
	 * Thus requiring an update in the graphics stack so that the user can see the new data.
	 */
	void updateGraphics();
private slots:
	// tcp
	void socketError(QAbstractSocket::SocketError);
	void socketConnected();
	void socketDisconnected();
	void socketReadyRead();
	// udp
	void udpSocketError(QAbstractSocket::SocketError);
	void udpSocketReadyRead();

	void handleDns(QHostInfo info);
	void handleJoinTimeout();
private:
	void handlePacket(std::unique_ptr<Packet::AbstractPacket> &p);
	void setJoinStatus(const JoinStatus s);
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
	/**
	 * @brief The current ping to the server
	 * This is updated with every Pong packet received.
	 */
	qint64 ping = 0;
	/**
	 * @brief The timer responsible for cancelling a join request, if it takes too long
	 */
	QTimer joinTimeoutTimer;
	/**
	 * @brief The index of the client in the server curver array
	 */
	int curverIndex = -1;
};
