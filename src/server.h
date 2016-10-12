#ifndef SERVER_H
#define SERVER_H
#include <QObject>
#include <QtNetwork>
#include "qcurver.h"


class Server : public QObject
{
	Q_OBJECT
public:
	explicit Server(QCurver **curver, quint16 port = 52552, QObject *parent = 0);
	~Server();
	void shutdown();
	void setAvailable(int index, bool newState);
	void start();

signals:
private slots:
	void readPendingDatagrams();
	void socketError(QAbstractSocket::SocketError socketError);
private:
	void initSocket();
	QUdpSocket *udpSocket;
	quint16 port;
	bool available[16]; //determines if curver[i] is an Online player. if it already is used is determined by clients
	QHostAddress *clients[16]; //is NULL if not connected, otherwise holds client information
	int isValidInput(QHostAddress *sender); //returns -1 on error, else returns the index of the curver that sender is in control of
	QCurver **curver;
	void turn(QHostAddress *sender, rotation r);
};

#endif // SERVER_H
