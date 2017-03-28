#include "aicontroller.h"

AIController::AIController(QCurver* curver, QCurver** player, int playerCount, int fieldsize) {
	this->curver = curver;
	this->player = player;
	this->playerCount = playerCount;
	this->fieldsize = fieldsize;
}

void AIController::makeMove() {
	maxPath.depth = 0;
	tempPath.splines[0] = curver->getPos();
	tempPath.angles[0] = curver->getAngle();
	findMaxPath(0, ROTATE_NONE);
	findMaxPath(0, ROTATE_LEFT);
	findMaxPath(0, ROTATE_RIGHT);
	curver->rotating = maxPath.rotations[0];
}

void AIController::findMaxPath(int depth, rotation r) {
	if (depth >= MAXDEPTH-1 || maxPath.depth == MAXDEPTH-2) { // abort if depth too large or maxdepth is already successfully reached
		return;
	}
	float a = tempPath.angles[depth] + TURNANGLE * (r-1);
	tempPath.angles[depth+1] = a;
	tempPath.splines[depth+1] = QPointF(tempPath.splines[depth].x() + LINELENGTH * cos(a), tempPath.splines[depth].y() + LINELENGTH * sin(a));
	tempPath.rotations[depth] = r;
	QPointF nextPos = tempPath.splines[depth+1];
	bool collision = !(nextPos.x() < fieldsize - PREFERREDWALLDISTANCE && nextPos.y() < fieldsize - PREFERREDWALLDISTANCE && nextPos.x() > PREFERREDWALLDISTANCE && nextPos.y() > PREFERREDWALLDISTANCE);
	QPointF posL = curver->translateWithThickness(tempPath.splines[depth], tempPath.angles[depth], 1);
	QPointF posR = curver->translateWithThickness(tempPath.splines[depth], tempPath.angles[depth], -1);
	QPointF nextPosL = curver->translateWithThickness(tempPath.splines[depth+1], tempPath.angles[depth+1], 1);
	QPointF nextPosR = curver->translateWithThickness(tempPath.splines[depth+1], tempPath.angles[depth+1], -1);
	for (int i = 0; i < playerCount && !collision; ++i) {
		collision = player[i]->checkforIntersection(posL, nextPosL) || player[i]->checkforIntersection(posR, nextPosR);
	}
	if (collision) {
		return;
	}
	// no collision
	if (depth > maxPath.depth) {
		// copy to maxPath
		maxPath.depth = depth;
//		for (int i = 0; i <= depth+1; ++i) {
//			maxPath.splines[i] = tempPath.splines[i];
//			maxPath.angles[i] = tempPath.angles[i];
//			if (i <= depth) {
//				maxPath.rotations[i] = tempPath.rotations[i];
//			}
//		}
		maxPath.rotations[0] = tempPath.rotations[0]; // currently we only need the first value of rotations
	}
	findMaxPath(depth + 1, ROTATE_NONE);
	findMaxPath(depth + 1, ROTATE_LEFT);
	findMaxPath(depth + 1, ROTATE_RIGHT);
}
