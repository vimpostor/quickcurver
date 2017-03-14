#ifndef SERVER_H
#define SERVER_H
#include <QObject>
#include <QtNetwork>
#include "qcurver.h"
#include <QTimer>
#include "network.h"
#include <QSignalMapper>
#include <qmath.h>
#define BROADCASTINTERVAL 33
#define MAXPOINTSSENTATONCE 16


class Server : public QObject
{
	Q_OBJECT
public:
	explicit Server(QCurver **curver, QObject *parent = 0);
	~Server();
	void init(quint16 port, QObject *qmlobject);
	void shutdown();
	void setAvailable(int index, bool newState);
	void start();
	void setPlayerCount(int playercount);
	void newRound();
	void broadcastChatMessage(QString username, QString message);
	QHostAddress *getServerIp();
	bool isReady();
	void transmitNewItem(QString iconName, QColor color, QPointF pos, int index);
	void useItem(int index);
	void cleanInstall();
	void curverDied(int index);
	void setName(int index, QString username);
signals:
private slots:
	void readPendingDatagrams();
	void udpSocketError(QAbstractSocket::SocketError socketError);
	void tcpSocketError(QAbstractSocket::SocketError socketError);
	void broadcast();
	void newConnection();
	void tcpReadyRead(int i);
private:
	QTcpSocket *clientConnections[MAXPLAYERCOUNT];
	void initUdpSocket();
	void initTcpServer();
	QUdpSocket *udpSocket;
	QTcpServer *tcpServer;
	quint16 port;
	bool available[MAXPLAYERCOUNT]; // determines if curver[i] is an Online player. if it already is used is determined by clientsTcp
	QHostAddress *clientsUdp[MAXPLAYERCOUNT]; // is NULL if not connected, otherwise holds client information
	QTcpSocket *clientsTcp[MAXPLAYERCOUNT]; // is NULL if not connected, otherwise holds an active tcp socket to client
	int isValidInput(QHostAddress *sender); // returns -1 on error, else returns the index of the curver that sender is in control of
	QCurver **curver;
	void turn(QHostAddress *sender, rotation r);
	bool started = false;
	QTimer *broadcastTimer;
	void sendToAllUdp(QByteArray *datagram);
	int playercount = 0;
	QHostAddress *serverIp;
	void setServerIp();
	int getFreePlace(); // returns index of first free slot. Returns -1, if no slot is free
	void sendToAllTcp(QByteArray *block); // sends block to all
	void transmitTcpMessage(QString message, QTcpSocket *s = NULL); // if s is not specified, sends to all
	void transmitTcpMessages(QString *messages, int length, QTcpSocket *s = NULL);
	void connectClient(QTcpSocket *client, int index);
	bool tryConnectingClient(QTcpSocket *client); // returns true on success, also sends ACCEPTED OR REJECTED to the client
	void disconnectClient(QTcpSocket *client, QString reason = ""); // if reason is specified, the client will be notified about the disconnection
	QDataStream in[MAXPLAYERCOUNT];
	QSignalMapper *tcpReadyReadSignalMapper;
	ServerSettings serverSettings;
	void updateServerSettings();
	QObject *qmlobject = NULL;
	Gui gui;
};

#endif // SERVER_H
