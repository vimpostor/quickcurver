#ifndef AICONTROLLER_H
#define AICONTROLLER_H

#include "qcurver.h"
#include <QPointF>
#include <QtMath>

#define WALLCOLLISION_PENALTY_FAR 5 //how bad is a collision with a wall far away
#define WALLCOLLISION_PENALTY_NEAR 30 //how bad is a collision with a wall very near
#define PLAYERCOLLISION_PENALTY_FAR 10 //how bad is a collision with another player far away
#define PLAYERCOLLISION_PENALTY_NEAR 20 //how bad is a collision with another player that is very close

#define ANGLEINCREMENT M_PI/16 //how accurate should the algorithm check for free passages
#define MAXANGLE M_PI/2 //maximum angle that a bot is willing to turn at a time

class AIController : public QObject
{
	Q_OBJECT
public:
    explicit AIController(QCurver* curver, QCurver** player, int playerCount, int fieldsize);
	void makeMove(float deltat);
private:
	QCurver* curver;
	QCurver** player;
	int playerCount;
    int fieldsize;
public slots:
};

#endif // AICONTROLLER_H
