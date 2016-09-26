#include "qcurver.h"

QCurver::QCurver(QSGNode *node, QColor color, int baseSpeed) {
	connect(this,SIGNAL(died(QCurver*)), this, SLOT(rollDieAnimation()));
	this->color = color;
	this->node = node;
	this->baseSpeed = baseSpeed;
	velocity = baseSpeed;
	material = new QSGFlatColorMaterial;
	material->setColor(color);
	segments[0] = new segment(color, node, material);
	rotateDirection(segment::randFloat()*2*M_PI);
	lastPoint = QPointF(segment::randInt(100,900),segment::randInt(100,900));
	lastnewSegment = QTime::currentTime();
	headnode = new headNode(lastPoint, material, node);
}

QCurver::~QCurver() {
	for (int i = 0; i < segmentcount; i++) {
		delete segments[i];
	}
	delete headnode;
}


void QCurver::progress(float deltat) {
	progressMutex.lock();
	if (rotating == ROTATE_LEFT) {
		turnLeft(deltat);
	} else if (rotating == ROTATE_RIGHT) {
		turnRight(deltat);
	} else { //ROTATE_NONE
		//do nothing
	}
	QPointF addedPoint = deltat * velocity * direction;
	QPointF newPoint = lastPoint + addedPoint;
	lastPoint = newPoint; //this maybe should be moved to the end of this block later, however moving it will at least break wallcollision
	if (!changingSegment) {
		if (lastnewSegment.msecsTo(QTime::currentTime()) > nextSegmentTime) {
			//new Segment
			segments[segmentcount] = new segment(color, node, material);
			segmentcount++;
			lastnewSegment = QTime::currentTime();
			nextSegmentTime = segment::randInt(1000,5000);
			changingSegment = true;
		} else {
			//no new Segment
			segments[segmentcount-1]->appendPoint(newPoint);
			bool wCollision = wallCollision();
			if (segments[segmentcount-1]->poscount > 1) {
				int pCollision = playerCollision();
			}
		}
	} else {
		//check if we should produce a new line again
		if (lastnewSegment.msecsTo(QTime::currentTime()) > segmentchangeTime) {
			lastnewSegment = QTime::currentTime();
			changingSegment = false;
			segmentchangeTime = 70;
		}
	}
	headnode->updatePosition(newPoint);
	progressMutex.unlock();
}


void QCurver::turnLeft(float deltat) {
	rotateDirection(deltat * -rotateVelocity);
}

void QCurver::turnRight(float deltat) {
	rotateDirection(deltat * rotateVelocity);
}

void QCurver::rotateDirection(float radians) {
	angle = angle + radians;
	if (angle > 2*M_PI) {
		angle -= 2*M_PI;
	} else if (angle < 0) {
		angle += 2*M_PI;
	}
	direction.setX(cos(angle));
	direction.setY(sin(angle));
}

bool QCurver::wallCollision() {
	bool c = false;
	if (lastPoint.x() < 7) { //left
		c = true;
	} else if (lastPoint.x() > 995) { //right
		c = true;
	} else if (lastPoint.y() < -7) { //top
		c = true;
	} else if (lastPoint.y() > 995) { //bottom
		c = true;
	} else { //no wall collision
		c = false;
	}
	if (c) {
		emit died(this);
	}
	return c;
}

int QCurver::playerCollision() {
	short int p = -1;
	emit requestIntersectionChecking(segments[segmentcount-1]->pos[segments[segmentcount-1]->poscount-2], lastPoint);

	if (p != -1) {
		emit died(this);
	}
	return p;
}

bool QCurver::checkforIntersection(QPointF a, QPointF b) {
	bool c = false;
	for (int i = 0; i < segmentcount && !c; i++) {
		c = segments[i]->checkforIntersection(a, b);
	}
	return c;
}

void QCurver::rollDieAnimation() {
	dieAnimation* d = new dieAnimation(lastPoint, node, material, this);
}

void QCurver::reset() {
	int oldSegmentcount = segmentcount;
	segmentcount = 0;
	for (int i = 0; i < oldSegmentcount; i++) {
		delete segments[i];
	}
	segments[0] = new segment(color, node, material);
	segmentcount = 1;
	rotating = ROTATE_NONE;
	rotateDirection(segment::randFloat()*2*M_PI);
	lastPoint = QPointF(segment::randInt(100,900),segment::randInt(100,900));
	lastnewSegment = QTime::currentTime();
	velocity = baseSpeed;
	thickness = 16;
	segmentchangeTime = 2000;
	nextSegmentTime = 2000;
	changingSegment = true;
	roundCount++;
}

QPointF QCurver::getPos() {
	return lastPoint;
}

void QCurver::doubleSpeed() {
	velocity = 2*velocity;
}

void QCurver::halfSpeed() {
	velocity = velocity / 2;
}

bool QCurver::verifyCorrectRound(int round) {
	return (round == this->roundCount);
}

QPointF QCurver::getEstimatedNextPos(float deltat, float angle, float velocityMultiplier) {
	return lastPoint + deltat * velocity * velocityMultiplier * getDirectionRotatedBy(angle);
}

QPointF QCurver::getDirectionRotatedBy(float angle) {
	return QPointF(cos(this->angle+angle), sin(this->angle+angle));
}

float QCurver::getAngle() {
	return angle;
}
