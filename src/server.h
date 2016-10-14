#ifndef SERVER_H
#define SERVER_H
#include <QObject>
#include <QtNetwork>
#include "qcurver.h"
#include <QTimer>
#define BROADCASTINTERVAL 50
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

signals:
private slots:
	void readPendingDatagrams();
	void socketError(QAbstractSocket::SocketError socketError);
	void broadcast();
private:
	void initSocket();
	QUdpSocket *udpSocket;
	quint16 port;
	bool available[MAXPLAYERCOUNT]; //determines if curver[i] is an Online player. if it already is used is determined by clients
	QHostAddress *clients[MAXPLAYERCOUNT]; //is NULL if not connected, otherwise holds client information
	int isValidInput(QHostAddress *sender); //returns -1 on error, else returns the index of the curver that sender is in control of
	QCurver **curver;
	void turn(QHostAddress *sender, rotation r);
	bool started = false;
	QTimer *broadcastTimer;
	void sendToAll(QByteArray *datagram);
	int currentSegment[16]; //determines the segment that the last broadcast sent positions from
	int currentPos[16]; //determines the index of the next position that we have to send from currentSegment
	int playercount = 0;
};

#endif // SERVER_H
