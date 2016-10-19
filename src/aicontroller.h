#ifndef AICONTROLLER_H
#define AICONTROLLER_H

#include "qcurver.h"
#include <QPointF>
#include <QtMath>

#define LOOKAHEADFACTOR 200

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
	float fmod(float a, float b);
public slots:
};

#endif // AICONTROLLER_H
