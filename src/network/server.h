#ifndef SERVER_H
#define SERVER_H
#include <QObject>
#include <QtNetwork>
#include <QTimer>
#include <QSignalMapper>
#include <memory>

#include "network.h"

#define ADMIN_NAME "Chat Bot"

class Server : public QObject
{
	Q_OBJECT
public:
	explicit Server();
	~Server();

	void broadcastCurverData();
	void broadcastChatMessage(QString username, QString message);
	void broadcastChatMessage(QString msg);
	void resetRound();
	void reListen(quint16 port);
public slots:
	void broadcastPlayerModel();
	void broadcastItemData(bool spawned, unsigned int sequenceNumber, int which, QPointF pos, Item::AllowedUsers allowedUsers, int collectorIndex);
private slots:
	void acceptError(QAbstractSocket::SocketError);
	void newConnection();
	void socketError(QAbstractSocket::SocketError);
	void socketDisconnect();
	void socketReadyRead();
private:
	void removePlayer(const QTcpSocket *s);
	void handlePacket(std::unique_ptr<Packet::AbstractPacket> &p, const QTcpSocket *s);
	void broadcastPacket(Packet::AbstractPacket &p);
	QTcpServer tcpServer;
	std::map<std::unique_ptr<QTcpSocket>, Curver *> clients;
	Curver *curverFromSocket(const QTcpSocket *s) const;
	bool resetDue = false;
};

#endif // SERVER_H
