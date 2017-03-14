#ifndef CLIENT_H
#define CLIENT_H
#include <QObject>
#include <QtNetwork>
#include "qcurver.h"
#include <QTimer>
#include <QSGNode>
#include "headnode.h"
#include <qsgflatcolormaterial.h>
#include "network.h"


class Client : public QObject
{
	Q_OBJECT
public:
	explicit Client(QQuickItem *parent);
	~Client();
	void sendKey(Qt::Key k);
	void releaseKey(Qt::Key k);
	void start(QSGNode *node, QObject *qmlobject, QString ip, int port = 52552);
	void shutdown(); // also sends [LEFT] to the server if already joined
	void requestSendMessage(QString message);
	void changeSettings(QString username, bool ready);
signals:
	void spawnItem(QString iconName, QColor color, QPointF pos, int index);
	void deleteItem(int index);
	void deleteAllItems();

private slots:
	void udpReadPendingDatagrams();
	void udpSocketError(QAbstractSocket::SocketError socketError);
	void timeout();
	void tcpReadyRead();
	void tcpSocketError(QAbstractSocket::SocketError socketError);
private:
	void initUdpSocket();
	void initTcpSocket();
	QQuickItem *parent = NULL;
	QUdpSocket *udpSocket = NULL;
	QTcpSocket *tcpSocket = NULL;
	QHostAddress *ip;
	quint16 port;
	quint16 myport = 55225;
	void join();
	QTimer *timeoutTimer;
	bool joined = false;
	QSGNode *node;
	headNode *headnodes[MAXPLAYERCOUNT];
	QCurver *curver[MAXPLAYERCOUNT];
	QDataStream in;
	void sendUdpMessage(QString msg);
	void sendTcpMessage(QString msg);
	ClientSettings settings;
	ServerSettings serverSettings;
	QObject *qmlobject = NULL;
	Gui gui;
};

#endif // CLIENT_H
