#include "aicontroller.h"

AIController::AIController(QCurver* curver, QCurver** player, int playerCount) {
	this->curver = curver;
	this->player = player;
	this->playerCount = playerCount;
}

void AIController::makeMove(float deltat) {
	//first lets care about the walls
	int rotatePriority[3] = {0, 0, 0};
	float angle = curver->getAngle();
	QPointF currentPos = curver->getPos();
	for (int collisionDistance = 70; collisionDistance <=130; collisionDistance+=60) {
		int distanceWeight = (collisionDistance == 70 ? 20 : 11); //a near collision has more weight to it than one far away
		QPointF estimatedPos = curver->getEstimatedNextPos(deltat, 0, collisionDistance);
		if (qAbs(estimatedPos.x() - 500) > 500 || qAbs(estimatedPos.y() - 500) > 500) { //we would collide with a wall
	//		int quadrant = estimatedPos.x() > 500 ? (estimatedPos.y() > 500 ? 0 : 3) : (estimatedPos.y() > 500 ? 1 : 2); //bottom right: 0, bottom left: 1, top left: 2, top right: 3
			int triangle = currentPos.x() > currentPos.y() ? (1000-currentPos.y() > currentPos.x() ? 3 : 0) : (1000-currentPos.y() > currentPos.x() ? 2 : 1); //right: 0, bot: 1, left: 2, top: 3
			float deltaAngle = angle - triangle * M_PI/2;
			if (deltaAngle > 0) {
				rotatePriority[ROTATE_RIGHT] += distanceWeight;
			} else {
				rotatePriority[ROTATE_LEFT] += distanceWeight;
			}
		} else { //we would not collide with a wall
			rotatePriority[ROTATE_NONE] += 8;
		}
	}

	//now lets care about other players
	bool c;
	float turnAngle;
	for (int direction = -1; direction < 2; direction+=2) {
		for (turnAngle = 0; turnAngle < M_PI/4 && turnAngle > -M_PI/4; turnAngle+=direction*M_PI/16) {
			c = false;
			//check if we would collide with someone with this angle
			for (int i = 0; i < playerCount && !c; i++) {
				c = player[i]->checkforIntersection(currentPos, curver->getEstimatedNextPos(deltat, turnAngle, 128));
			}
			if (!c) { //when we found the first acceptable angle we break
				break;
			}
		}
		rotatePriority[direction+1] += (M_PI/4-turnAngle)/M_PI*70; //the higher the angle that we have to rotate, the less likely we want to take that route (linearly interpolated)
	}


	if (rotatePriority[ROTATE_LEFT] == rotatePriority[ROTATE_RIGHT]) {
		rotatePriority[ROTATE_NONE] += 100;
	}
	//take the rotation with highest priority
	int maxIndex = 1; //in case that all priorities are equal, we prefer to not rotate
	int maxPriority = 0;
	for (int i = 0; i < 3; i++) {
		if (rotatePriority[i] > maxPriority) {
			maxIndex = i;
			maxPriority = rotatePriority[maxIndex];
		}
	}
	curver->rotating = static_cast<rotation>(maxIndex);
}

float AIController::fmod(float a, float b) {
	while (a < 0) {
		a += b;
	}
	while (a >= b) {
		a -= b;
	}
	return a;
}
