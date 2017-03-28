#ifndef AICONTROLLER_H
#define AICONTROLLER_H

#include "qcurver.h"
#include <QPointF>
#include <QtMath>

#define MAXDEPTH 4
#define LINELENGTH 25
#define TURNANGLE 0.35
#define PREFERREDWALLDISTANCE 10

class AIController : public QObject
{
	Q_OBJECT
public:
	explicit AIController(QCurver* curver, QCurver** player, int playerCount, int fieldsize);
	void makeMove();
private:
	QCurver* curver;
	QCurver** player;
	int playerCount;
	int fieldsize;
	struct Path {
		QPointF splines[MAXDEPTH];
		float angles[MAXDEPTH];
		rotation rotations[MAXDEPTH - 1];
		int depth = 0;
	} maxPath, tempPath;
	void findMaxPath(int depth, rotation r);
};

#endif // AICONTROLLER_H
