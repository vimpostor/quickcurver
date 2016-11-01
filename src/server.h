#ifndef SERVER_H
#define SERVER_H
#include <QObject>
#include <QtNetwork>
#include "qcurver.h"
#include <QTimer>
#define BROADCASTINTERVAL 33
#define MAXPLAYERCOUNT 16


class Server : public QObject
{
	Q_OBJECT
public:
	explicit Server(QCurver **curver, quint16 port = 52552, QObject *parent = 0);
	~Server();
	void shutdown();
	void setAvailable(int index, bool newState);
	void start();
	void setPlayerCount(int playercount);
    void newRound();

signals:
    void playerStatusChanged(int index, QString s);
private slots:
	void readPendingDatagrams();
	void socketError(QAbstractSocket::SocketError socketError);
	void broadcast();
    void newConnection();
private:
    QTcpSocket *clientConnections[MAXPLAYERCOUNT];
    void initUdpSocket();
    void initTcpServer();
	QUdpSocket *udpSocket;
    QTcpServer *tcpServer;
	quint16 port;
	bool available[MAXPLAYERCOUNT]; //determines if curver[i] is an Online player. if it already is used is determined by clients
	QHostAddress *clients[MAXPLAYERCOUNT]; //is NULL if not connected, otherwise holds client information
	int isValidInput(QHostAddress *sender); //returns -1 on error, else returns the index of the curver that sender is in control of
	QCurver **curver;
	void turn(QHostAddress *sender, rotation r);
	bool started = false;
	QTimer *broadcastTimer;
	void sendToAll(QByteArray *datagram);
	int playercount = 0;
    QHostAddress *serverIp;
	void setServerIp();
};

#endif // SERVER_H
