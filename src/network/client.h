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
public:
	Client();
	void connectToHost(QHostAddress &addr, quint16 port);
	void sendChatMessage(QString msg);
	void sendPlayerModel();
	void processKey(Qt::Key key, bool release);
signals:
	/**
	 * @brief Emitted when the connection status changed
	 * @param connected Whether the new status is connected
	 */
	void connectedToServerChanged(bool connected);
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
private slots:
	void socketError(QAbstractSocket::SocketError);
	void socketConnected();
	void socketDisconnected();
	void socketReadyRead();
private:
	void handlePacket(std::unique_ptr<Packet::AbstractPacket> &p);
	/**
	 * @brief The socket to communicate with
	 */
	QTcpSocket socket;
	/**
	 * @brief A data stream belonging to Client::socket
	 */
	QDataStream in;
};

#endif // CLIENT_H
