#ifndef CLIENT_H
#define CLIENT_H
#include <QObject>
#include <QtNetwork>
#include "qcurver.h"
#include <QTimer>
#include <QSGNode>
#include "headnode.h"
#include <qsgflatcolormaterial.h>
#define MAXPLAYERCOUNT 16


class Client : public QObject
{
	Q_OBJECT
public:
    explicit Client(QString ip, QSGNode *node, int port = 52552, QObject *parent = 0);
	~Client();
	void sendKey(Qt::Key k);
	void releaseKey(Qt::Key k);
	void shutdown();
    void requestSendMessage(QString username, QString message);
signals:
	void joinStatusChanged(QString s);
    void updateGUI();
    void sendMessage(QString username, QString message);

private slots:
    void udpReadPendingDatagrams();
    void udpSocketError(QAbstractSocket::SocketError socketError);
	void timeout();
    void tcpReadyRead();
    void tcpSocketError(QAbstractSocket::SocketError socketError);
private:
    void initUdpSocket();
    void initTcpSocket();
	QUdpSocket *udpSocket;
    QTcpSocket *tcpSocket;
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
};

#endif // CLIENT_H
