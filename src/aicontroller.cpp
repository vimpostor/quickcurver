#include "aicontroller.h"

AIController::AIController(QCurver* curver, QCurver** player, int playerCount, int fieldsize) {
	this->curver = curver;
	this->player = player;
	this->playerCount = playerCount;
    this->fieldsize = fieldsize;
}

void AIController::makeMove(float deltat) {
	//first lets care about the walls
	int rotatePriority[3] = {0, 0, 0};
	float angle = curver->getAngle();
	QPointF currentPos = curver->getPos();
	for (int collisionDistance = 45; collisionDistance <=100; collisionDistance+=30) {
		int distanceWeight = (collisionDistance == 45 ? WALLCOLLISION_PENALTY_NEAR : WALLCOLLISION_PENALTY_FAR); //a near collision has more weight to it than one far away
		QPointF estimatedPos = curver->getEstimatedNextPos(deltat, 0, collisionDistance);
        if (qAbs(estimatedPos.x() - fieldsize/2) > fieldsize/2 || qAbs(estimatedPos.y() - fieldsize/2) > fieldsize/2) { //we would collide with a wall
	//		int quadrant = estimatedPos.x() > 500 ? (estimatedPos.y() > 500 ? 0 : 3) : (estimatedPos.y() > 500 ? 1 : 2); //bottom right: 0, bottom left: 1, top left: 2, top right: 3
            int triangle = currentPos.x() > currentPos.y() ? (fieldsize-currentPos.y() > currentPos.x() ? 3 : 0) : (fieldsize-currentPos.y() > currentPos.x() ? 2 : 1); //right: 0, bot: 1, left: 2, top: 3
			float deltaAngle = angle - triangle * M_PI/2;
			if (triangle == 0 && angle > M_PI) {
				deltaAngle = angle - 2*M_PI; //fixes anomaly at 2M_PI->0 jump
			}
			if (deltaAngle > 0) {
				rotatePriority[ROTATE_RIGHT] += distanceWeight; //we wanna rotate right because that is the shorter option
				rotatePriority[ROTATE_LEFT] += distanceWeight/4;
			} else {
				rotatePriority[ROTATE_LEFT] += distanceWeight;
				rotatePriority[ROTATE_RIGHT] += distanceWeight/4;
			}
		} else { //we would not collide with a wall
			rotatePriority[ROTATE_NONE] += 8;
		}
	}

	//now lets care about other players
	bool c;
	float turnAngle;
	QPointF perpendicularVector = curver->getDirectionRotatedBy(-M_PI/2);
	QPointF currentPosL = currentPos + curver->thickness * perpendicularVector;
	QPointF currentPosR = currentPos - curver->thickness * perpendicularVector;
    for (int collisionDistance = 40; collisionDistance <= 90; collisionDistance+=50) {
        int distanceWeight = collisionDistance == 40 ? PLAYERCOLLISION_PENALTY_NEAR : PLAYERCOLLISION_PENALTY_FAR;
		for (int direction = -1; direction < 2; direction+=2) {
			for (turnAngle = 0; qAbs(turnAngle) < MAXANGLE; turnAngle+=direction*ANGLEINCREMENT) {
				c = false;
				//check if we would collide with someone with this angle
				for (int i = 0; i < playerCount && !c; i++) {
					// c = player[i]->checkforIntersection(currentPos, curver->getEstimatedNextPos(deltat, turnAngle, 80));
					c = player[i]->checkforIntersection(currentPosL, curver->getEstimatedNextPos(deltat, turnAngle, collisionDistance, -1)); //first check left side
                    c |= player[i]->checkforIntersection(currentPosR, curver->getEstimatedNextPos(deltat, turnAngle, collisionDistance, 1)); //right side
				}
				if (!c) { //when we found the first acceptable angle we break
					break;
				}
			}
            rotatePriority[direction+1] += (MAXANGLE-turnAngle)/M_PI*distanceWeight; //the higher the angle that we have to rotate, the less likely we want to take that route (linearly interpolated)
		}
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
