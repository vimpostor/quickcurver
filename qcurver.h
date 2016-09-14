#ifndef QCURVER_H
#define QCURVER_H
#include <QtQuick/QQuickItem>
#include <QtQuick/qsgnode.h>
#include <QtQuick/qsgflatcolormaterial.h>
#include <QtMath>
#include <QTime>
#include "segment.h"
#include <QMutex>
#include "headnode.h"
#include "segment.h"
#include "dieanimation.h"

#define MAXSEGMENTCOUNT 60

enum rotation {
	ROTATE_LEFT,
	ROTATE_NONE,
	ROTATE_RIGHT
};

class QCurver : public QObject
{
    Q_OBJECT
public:
	explicit QCurver(QColor color, QSGNode* node);
    ~QCurver();
	segment* segments[MAXSEGMENTCOUNT];
	int segmentcount = 1;
	enum rotation rotating = ROTATE_NONE;
	bool checkforIntersection(QPointF a, QPointF b);
	void reset();
	QPointF getPos();
	void doubleSpeed();
	void halfSpeed();

signals:
	void died(QCurver* who);
	void requestIntersectionChecking(QPointF a, QPointF b);

public slots:
	void progress(float deltat);
	void turnLeft(float deltat);
	void turnRight(float deltat);

private slots:
	void rollDieAnimation();
private:
	QPointF direction = QPointF(1,1);
	QPointF lastPoint;
	float angle = 0;
	int velocity = 128;
	void rotateDirection(float radians);
	float rotateVelocity = M_PI;
	QTime lastnewSegment;
	int thickness = 16;
	int segmentchangeTime = 2000; //amount of time in milliseconds that you produce no line between segments
	int nextSegmentTime = 2000; //amount of time that you produce a line
	QSGNode* node;
	QMutex progressMutex;
	bool changingSegment = true;
	headNode* headnode;
	QColor color;
	QSGFlatColorMaterial* material;
	bool wallCollision();
	int playerCollision(); //returns -1  if no collision, else returns the number of the player with whom we are colliding
};

#endif // QCURVER_H
