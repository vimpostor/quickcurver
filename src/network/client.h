#ifndef CLIENT_H
#define CLIENT_H
#include <QObject>
#include <QtNetwork>
#include <QTimer>

#include "network.h"


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
	void connectedToServerChanged(bool connected);
	void integrateItem(bool spawned, unsigned int sequenceNumber, int which, QPointF pos, Item::AllowedUsers allowedUsers, int collectorIndex);
	void resetRound();
private slots:
	void socketError(QAbstractSocket::SocketError);
	void socketConnected();
	void socketDisconnected();
	void socketReadyRead();
private:
	void handlePacket(std::unique_ptr<Packet::AbstractPacket> &p);
	QTcpSocket socket;
	QDataStream in;
};

#endif // CLIENT_H
