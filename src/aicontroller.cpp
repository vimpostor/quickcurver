#include "aicontroller.h"

AIController::AIController(QCurver* curver, QCurver** player, int playerCount) {
	this->curver = curver;
	this->player = player;
	this->playerCount = playerCount;
}

void AIController::makeMove(float deltat) {
	float angle = 0;
	QPointF currPos = curver->getPos();
	bool c;
	float currAngle = curver->getAngle();
	if (currPos.x() <= 50) { //left wall
		if (currAngle >= M_PI && currAngle <= 1.5*M_PI) {
			angle = M_PI/8;
		} else if (currAngle <= M_PI && currAngle >= 0.5*M_PI) {
			angle = -M_PI/8;
		}
	} else if (currPos.y() <= 50) { //top wall
		if (currAngle >= 1.5*M_PI && currAngle <= 2*M_PI) {
			angle = M_PI/8;
		} else if (currAngle <= 1.5*M_PI && currAngle >= M_PI) {
			angle = -M_PI/8;
		}
	} else if (currPos.x() >= 950) { //right wall
		if (currAngle >= 0 && currAngle <= M_PI/2) {
			angle = M_PI/8;
		} else if (currAngle <= 2*M_PI && currAngle >= 1.5*M_PI) {
			angle = -M_PI/8;
		}
	} else if (currPos.y() >= 950) { //bottom wall
		if (currAngle >= M_PI/2 && currAngle <= M_PI) {
			angle = M_PI/8;
		} else if (currAngle <= M_PI/2 && currAngle >= 0) {
			angle = -M_PI/8;
		}
	}
	float angleStep = M_PI/8;
	if (angle < 0) {
		angleStep = -M_PI/8;
	}
	for (; (angle < 0 && angle > -M_PI) || (angle >= 0 && angle < M_PI); angle = (angle+angleStep)) {
		c = false;
		//check if we would collide with someone with this angle
		for (int i = 0; i < playerCount && !c; i++) {
			c = player[i]->checkforIntersection(currPos, curver->getEstimatedNextPos(deltat, angle, 200));
		}
		if (!c) {
			break;
		}
	}
	if (angle == 0) {
		curver->rotating = ROTATE_NONE;
	} else if (angle > 0) {
		curver->rotating = ROTATE_RIGHT;
	} else { //angle < 0
		curver->rotating = ROTATE_LEFT;
	}
}
