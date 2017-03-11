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
#include "cleaninstallanimation.h"

#define MAXSEGMENTCOUNT 60

enum rotation {
	ROTATE_LEFT,
	ROTATE_NONE,
	ROTATE_RIGHT
};

class QCurver : public QObject {
	Q_OBJECT
public:
	explicit QCurver(QSGNode* node, QColor color, int baseSpeed, int fieldsize);
	explicit QCurver(QSGNode* node, QColor color);
	~QCurver();
	segment* segments[MAXSEGMENTCOUNT];
	int segmentcount = 0;
	enum rotation rotating = ROTATE_NONE;
	bool checkforIntersection(QPointF a, QPointF b);
	void reset(); //completely resets for next round
	void cleanInstall(); //resets in the round (like from a cleaninstall item)
	QPointF getPos(int offset = 0);
	void doubleSpeed();
	void halfSpeed();
	bool verifyCorrectRound(int round);
	QPointF getEstimatedNextPos(float deltat, float angle, float velocityMultiplier = 1, float thicknessMultiplier=0);
	float getAngle();
	void doubleThickness();
	void halfThickness();
	QColor getColor();
	bool hasUnsyncedSegPoints();
	QPointF readUnsyncedSegPoint();
	bool moveToNextSegment(); // attempts to move to next segment, returns true on success, false on failure
	void clientNewSegment();
	void clientAddPoint(QPointF p);
	void clientReset();
	QPointF getDirectionRotatedBy(float angle);
	int thickness = 4;
	bool alive = true;
	void die();
	void goInvisible(int duration);
	int score = 0;
	int roundScore = 0;
	void increaseScore();
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
	int baseSpeed = 128;
	void rotateDirection(float radians);
	float rotateVelocity = M_PI;
	QTime lastnewSegment;
	int defaultSegmentChangeTime = 128;
	int segmentchangeTime = 2000; //amount of time in milliseconds that you produce no line between segments
	int nextSegmentTime = 2000; //amount of time that you produce a line
	QSGNode* node;
	QMutex progressMutex;
	bool changingSegment = true;
	headNode* headnode;
	QColor color;
	QSGFlatColorMaterial* material;
	bool wallCollision();
	int roundCount = 0;
	int playerCollision(); //returns -1  if no collision, else returns the number of the player with whom we are colliding
	int clientSegment = -1; //last segment synchronized
	int clientPoscount = -1; //last pos synchronized
	int fieldsize = 1000;
};

#endif // QCURVER_H
