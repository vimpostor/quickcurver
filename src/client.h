#ifndef CLIENT_H
#define CLIENT_H
#include <QObject>
#include <QtNetwork>
#include "qcurver.h"


class Client : public QObject
{
	Q_OBJECT
public:
	explicit Client(QString ip, int port = 52552, QObject *parent = 0);
	~Client();
	void sendKey(Qt::Key k);
	void releaseKey(Qt::Key k);
	void shutdown();
signals:
	void joinStatusChanged(QString s);

private slots:
	void readPendingDatagrams();
	void socketError(QAbstractSocket::SocketError socketError);
private:
	void initSocket();
	QUdpSocket *udpSocket;
	QHostAddress *ip;
	quint16 port;
	quint16 myport = 55225;
	void join();
};

#endif // CLIENT_H
