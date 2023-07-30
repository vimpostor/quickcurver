#pragma once

#include <QObject>
#include <QSignalMapper>
#include <QTimer>
#include <QtNetwork>
#include <array>
#include <memory>

#include "network.hpp"

#define ADMIN_NAME "Chat Bot"

/**
 * @brief A Server network instance
 *
 * Handles the network communication from the server side
 */
class Server : public QObject {
	Q_OBJECT
public:
	explicit Server();
	~Server();

	void broadcastCurverData();
	void broadcastChatMessage(QString username, QString message);
	void broadcastChatMessage(QString msg);
	void broadcastSettings();
	void resetRound();
	void reListen(quint16 port);
public slots:
	void broadcastPlayerModel();
	void broadcastItemData(bool spawned, unsigned int sequenceNumber, int which, QPointF pos, Item::AllowedUsers allowedUsers, int collectorIndex);
private slots:
	// tcpServer
	void acceptError(QAbstractSocket::SocketError);
	void newConnection();
	// tcpSocket
	void socketError(QAbstractSocket::SocketError);
	void socketDisconnect();
	void socketReadyRead();
	// udpSocket
	void udpSocketError(QAbstractSocket::SocketError);
	void udpSocketReadyRead();
private:
	void removePlayer(const QTcpSocket *s);
	void handlePacket(std::unique_ptr<Packet::AbstractPacket> &p, const QTcpSocket *s = nullptr, FullNetworkAddress sender = {});
	void broadcastPacket(Packet::AbstractPacket &p, bool udp = false);
	int getCurverIndex(const FullNetworkAddress peer);
	/**
	 * @brief The server instance that handles every incoming connection
	 */
	QTcpServer tcpServer;
	/**
	 * @brief The UDP server
	 */
	QUdpSocket udpSocket;
	/**
	 * @brief The mapping between TCP sockets and their Curver instances
	 */
	std::map<std::unique_ptr<QTcpSocket>, Curver *> clients;
	Curver *curverFromSocket(const QTcpSocket *s) const;
	/**
	 * @brief Whether the round has to be reset
	 *
	 * Usually the next update of Curver data resets this flag, after sending the reset bit to every Client
	 */
	bool resetDue = false;
	/**
	 * @brief The amount of times that curver data was broadcasted
	 *
	 * This value is used together with Settings::networkCurverBlock to reduce used network bandwidth
	 */
	unsigned dataBroadcastIteration = 0;
	/**
	 * @brief The UDP addresses from all clients
	 */
	std::vector<FullNetworkAddress> udpAddresses;
};
