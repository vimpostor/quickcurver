#ifndef GAME_H
#define GAME_H
#include <QtQuick/QQuickItem>
#include <QtQuick/qsgnode.h>
#include <QtQuick/qsgflatcolormaterial.h>
#include "qcurver.h"
#include <QTimer>
#include <QTime>
#include <QKeyEvent>
#include <QObject>
#include "qcurver.h"
#include <QSGGeometry>
#include "curveitem.h"
#include "fastitem.h"
#include <QQuickView>
#include "wallnode.h"
#include <QObject>
#include "aicontroller.h"
#include "cleaninstallitem.h"
#include "fatteritem.h"
#include "server.h"
#include "client.h"
#define MAXPLAYERCOUNT 16
#define MAXITEMCOUNT 20
#define AIINTERVAL 4
#define ITEMVARIETY 6


class Game : public QQuickItem
{
    Q_OBJECT
public:
    explicit Game(QQuickItem *parent = 0);
	Q_INVOKABLE void sendKey(Qt::Key k);
	Q_INVOKABLE void releaseKey(Qt::Key k);
	Q_INVOKABLE void start();
	Q_INVOKABLE void clientStart(QString ip, int port);
	Q_INVOKABLE void setColor(int index, QColor color);
	Q_INVOKABLE void setControls(int index, Qt::Key k, bool isRight);
	Q_INVOKABLE void setName(int index, QString newName);
	Q_INVOKABLE void setRoundTimeout(int seconds);
	Q_INVOKABLE void setBaseSpeed(int baseSpeed);
	Q_INVOKABLE void setController(int index, int newControllerState);
	Q_INVOKABLE void setItemPriority(int index, int newPriority);
	Q_INVOKABLE void close();
	QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *);
    ~Game();
	Q_INVOKABLE void addPlayer();
	short int playercount = 2;
	void setQmlObject(QObject *o);

signals:
public slots:
	void progress();
	void curverDied(QCurver* who);
	void checkforIntersection(QPointF a, QPointF b);
private slots:
	void startNextRound();
	void setJoinStatus(QString s);

private:
	QCurver* curver[MAXPLAYERCOUNT];
	bool alive[MAXPLAYERCOUNT];
	QColor colors[MAXPLAYERCOUNT];
	Qt::Key controls[MAXPLAYERCOUNT][2]; //first one is left key, second one is right key
	bool controlledByAI[MAXPLAYERCOUNT];
	bool controlledByNetwork[MAXPLAYERCOUNT];
	AIController* ai[MAXPLAYERCOUNT];
	Server *server;
	Client *client;
	bool host = true;
	int score[MAXPLAYERCOUNT];
	CurveItem* items[MAXITEMCOUNT];
	QString names[MAXITEMCOUNT];
	int roundScore[MAXITEMCOUNT];
	QTimer* timer;
	QTimer* nextRoundTimer;
	QTime lastTime;
	QSGNode *node = 0;
	int roundCount = 0;
	void nextRound();
	int nextItemSpawn; //time in milliseconds
	QTime lastItemSpawn;
	wallNode *wall;
	int timerInterval = 16;
	int roundTimeout = 2000; //milliseconds
	int baseSpeed = 128;
	int frameCount = 0;
	QObject *qmlobject;
	void increaseScore(int index);
	int itemPriority[ITEMVARIETY] = {3, 0, 1, 0, 0, 2};
	int itemPrioritySum = 0; //a for loop will set this in the start() method
};

#endif // GAME_H
