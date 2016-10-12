#ifndef CLIENT_H
#define CLIENT_H
#include <QObject>
#include <QtNetwork>
#include "qcurver.h"
#include <QTimer>


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
	void timeout();
private:
	void initSocket();
	QUdpSocket *udpSocket;
	QHostAddress *ip;
	quint16 port;
	quint16 myport = 55225;
	void join();
	QTimer *timeoutTimer;
	bool joined = false;
};

#endif // CLIENT_H
