#ifndef AICONTROLLER_H
#define AICONTROLLER_H

#include "qcurver.h"
#include <QPointF>

class AIController : public QObject
{
	Q_OBJECT
public:
	explicit AIController(QCurver* curver, QCurver** player, int playerCount);
	void makeMove(float deltat);
private:
	QCurver* curver;
	QCurver** player;
	int playerCount;
public slots:
};

#endif // AICONTROLLER_H
