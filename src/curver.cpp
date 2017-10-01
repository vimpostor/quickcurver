#include "curver.h"

#define SPAWN_WALL_THRESHOLD 50
#define SPAWN_INVINCIBLE_DURATION 1000
#define CLEAN_INVINCIBLE_DURATION 100
#define SEGMENT_USE_TIME_MIN 5000
#define SEGMENT_USE_TIME_MAX 8000
#define SEGMENT_CHANGE_TIME 300

Curver::Curver(QSGNode *parentNode)
{
	this->parentNode = parentNode;

	nextSegmentEvent = QTime::currentTime();
	setColor(Util::randColor());
	// random initial rotation
	headNode = std::make_unique<HeadNode>(parentNode, &material);
	resetRound();
}

Curver::~Curver()
{
}

void Curver::setColor(const QColor color)
{
	this->color = color;
	material.setColor(color);
}

QColor Curver::getColor() const
{
	return this->color;
}

void Curver::setLeftKey(const Qt::Key key)
{
	leftKey = key;
}

Qt::Key Curver::getLeftKey() const
{
	return this->leftKey;
}

void Curver::setRightKey(const Qt::Key key)
{
	rightKey = key;
}

Qt::Key Curver::getRightKey() const
{
	return this->rightKey;
}

const std::vector<std::unique_ptr<Segment> > &Curver::getSegments() const
{
	return this->segments;
}

QPointF Curver::getPos() const
{
	return this->lastPos;
}

QPointF Curver::getDirection() const
{
	return this->direction;
}

float Curver::getAngle() const
{
	return angle;
}

bool Curver::isChangingSegment() const
{
	return this->changingSegment;
}

void Curver::processKey(Qt::Key key, bool release)
{
	if (controller != CONTROLLER_LOCAL) {
		return;
	}
	if (release) {
		rotation = ROTATE_NONE;
	} else {
		if (key == leftKey) {
			rotation = ROTATE_LEFT;
		} else if (key == rightKey) {
			rotation = ROTATE_RIGHT;
		}
	}
}

void Curver::start()
{
	resetRound();
}

bool Curver::checkForIntersection(std::vector<std::unique_ptr<Curver> > &curvers, QPointF a, QPointF b) const
{
	for (auto &i : curvers) {
		const auto &otherSegments = i->getSegments();
		if (otherSegments.end() != Util::find_if(otherSegments, [&](auto &segment){return segment->checkForIntersection(a, b);})) {
			return true;
		}
	}
	return false;
}

void Curver::checkForWall()
{
	QPoint dimension = Settings::getSingleton().getDimension();
	if (alive && !changingSegment && (lastPos.x() < 0 || lastPos.x() > dimension.x() || lastPos.y() < 0 || lastPos.y() > dimension.y())) {
		die();
	}
}

void Curver::cleanInstall()
{
	prepareSegmentEvent(true, CLEAN_INVINCIBLE_DURATION, CLEAN_INVINCIBLE_DURATION);
	segments.clear();
}

void Curver::increaseScore()
{
	++roundScore;
	++totalScore;
}

void Curver::resetRound()
{
	segments.clear();
	// random start position
	QPoint dimension = Settings::getSingleton().getDimension();
	lastPos = QPointF(Util::randInt(SPAWN_WALL_THRESHOLD, dimension.x() - SPAWN_WALL_THRESHOLD), Util::randInt(SPAWN_WALL_THRESHOLD, dimension.y() - SPAWN_WALL_THRESHOLD));
	rotate(Util::rand() * 2 * M_PI);
	prepareSegmentEvent(true, SPAWN_INVINCIBLE_DURATION, SPAWN_INVINCIBLE_DURATION);
	roundScore = 0;
	alive = true;
}

bool Curver::isAlive() const
{
	return alive;
}

void Curver::appendPoint(const QPointF pos, const bool changingSegment)
{
	if (!changingSegment && oldChangingSegment) {
		segments.push_back(std::make_unique<Segment>(parentNode, &material, thickness));
	}
	headNode->setPosition(pos);
	if (!changingSegment && pos != lastPos) {
		const QPointF diff = pos - lastPos;
		const double length = sqrt(QPointF::dotProduct(diff, diff));
		float angle = acos(diff.x() / length);
		if (diff.y() < 0) {
			angle = -1 * angle;
		}
		segments.back()->appendPoint(pos, angle);
	}
	oldChangingSegment = changingSegment;
	lastPos = pos;
}

void Curver::prepareSegmentEvent(bool changingSegment, int lower, int upper)
{
	nextSegmentEvent = QTime::currentTime().addMSecs(Util::randInt(lower, upper));
	this->changingSegment = changingSegment;
}

void Curver::progress(int deltat, std::vector<std::unique_ptr<Curver> > &curvers)
{
	if (nextSegmentEvent <= QTime::currentTime()) {
		if (changingSegment) {
			// spawn a new segment
			segments.push_back(std::make_unique<Segment>(parentNode, &material, thickness));
			prepareSegmentEvent(false, SEGMENT_USE_TIME_MIN, SEGMENT_USE_TIME_MAX);
		} else {
			// plan a new segment spawn
			prepareSegmentEvent(true, SEGMENT_CHANGE_TIME, SEGMENT_CHANGE_TIME);
		}
	}
	secondLastPos = lastPos;
	if (rotation == ROTATE_LEFT) {
		rotate(-deltat * rotateVelocity);
	} else if (rotation == ROTATE_RIGHT) {
		rotate(deltat * rotateVelocity);
	}
	lastPos += deltat * velocity * direction;
	if (headVisible) {
		headNode->setPosition(lastPos);
	} else {
		// it is not possible to have both head invisible and not changing segment
		// check for collision
		if (checkForIntersection(curvers, secondLastPos, lastPos)) {
			die();
		}
	}
	if (!changingSegment) {
		segments.back()->appendPoint(lastPos, angle);
		// check for collision
		if (checkForIntersection(curvers, secondLastPos, lastPos)) {
			die();
		}
	}
}

void Curver::rotate(float radians)
{
	angle += radians;
	if (angle < 0) {
		angle += 2 * M_PI;
	} else if (angle > 2 * M_PI) {
		angle -= 2 * M_PI;
	}
	direction.setX(cos(angle));
	direction.setY(sin(angle));
}

void Curver::die()
{
	alive = false;
	(void) new Explosion(lastPos, parentNode, &material, this);
	emit died();
}

bool operator <(const std::unique_ptr<Curver> &l, const std::unique_ptr<Curver> &r)
{
	return l->totalScore < r->totalScore;
}
