#include "curver.h"

#define SPAWN_WALL_THRESHOLD 50
#define SPAWN_INVINCIBLE_DURATION 1000
#define CLEAN_INVINCIBLE_DURATION 100
#define SEGMENT_USE_TIME_MIN 5000
#define SEGMENT_USE_TIME_MAX 8000
#define SEGMENT_CHANGE_TIME 300

/**
 * @brief Constructs a Curver object that belongs to \a parentNode in the scene graph
 * @param parentNode The parent node in the scene graph
 */
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

/**
 * @brief Sets the color of the Curver
 * @param color The new color
 */
void Curver::setColor(const QColor color)
{
	this->color = color;
	material.setColor(color);
}

/**
 * @brief Returns the color of the Curver
 * @return The color
 */
QColor Curver::getColor() const
{
	return this->color;
}

/**
 * @brief Sets the left key of the Curver
 *
 * This key is used to rotate counter clockwise
 * @param key The left key
 */
void Curver::setLeftKey(const Qt::Key key)
{
	leftKey = key;
}

/**
 * @brief Returns the left key
 * @return The left key
 */
Qt::Key Curver::getLeftKey() const
{
	return this->leftKey;
}

/**
 * @brief Sets the right key of the Curver
 *
 * This key is used to rotate clockwise
 * @param key The right key
 */
void Curver::setRightKey(const Qt::Key key)
{
	rightKey = key;
}

/**
 * @brief Returns the right key
 * @return The right key (not the wrong one! :) )
 */
Qt::Key Curver::getRightKey() const
{
	return this->rightKey;
}

/**
 * @brief Returns the segments of this Curver
 * @return The segments
 */
const std::vector<std::unique_ptr<Segment> > &Curver::getSegments() const
{
	return this->segments;
}

/**
 * @brief Returns the current position
 * @return The current position
 */
QPointF Curver::getPos() const
{
	return this->lastPos;
}

/**
 * @brief Returns the current direction vector
 * @return The direction
 */
QPointF Curver::getDirection() const
{
	return this->direction;
}

/**
 * @brief Returns the current angle
 * @return The angle
 */
float Curver::getAngle() const
{
	return angle;
}

/**
 * @brief Determines if the Curver is currently changing segments
 * @return \c True, iif changing segments at the moment
 */
bool Curver::isChangingSegment() const
{
	return this->changingSegment;
}

/**
 * @brief Processes the given key
 *
 * If the key is responsible for controlling the Curver, this triggers a rotation accordingly.
 * @param key The key to process
 * @param release If the key was pressed or released
 */
void Curver::processKey(Qt::Key key, bool release)
{
	if (controller != Controller::CONTROLLER_LOCAL || ((key != leftKey) && (key != rightKey))) {
		return;
	}
	if (release) {
		rotation = Rotation::ROTATE_NONE;
	} else if (key == leftKey) {
		rotation = Rotation::ROTATE_LEFT;
	} else if (key == rightKey) {
		rotation = Rotation::ROTATE_RIGHT;
	}
}

/**
 * @brief Notify that the game has started
 */
void Curver::start()
{
	resetRound();
}

/**
 * @brief Checks, if any Curver collides with the line from \a a to \a b
 * @param curvers All Curvers
 * @param a The start point of the line
 * @param b The end point of the line
 * @return \c True, iif the line collides
 */
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

/**
 * @brief Check, if the Curver collides with a wall
 *
 * Triggers Curver::die(), if it does in fact collide
 */
void Curver::checkForWall()
{
	QPoint dimension = Settings::getSingleton().getDimension();
	if (alive && !changingSegment && (lastPos.x() < 0 || lastPos.x() > dimension.x() || lastPos.y() < 0 || lastPos.y() > dimension.y())) {
		die();
	}
}

/**
 * @brief Triggers a cleaninstall of the Curver
 *
 * Erases all previous segments and immediately inits a new segment.
 */
void Curver::cleanInstall()
{
	prepareSegmentEvent(true, CLEAN_INVINCIBLE_DURATION, CLEAN_INVINCIBLE_DURATION);
	// spawn cleaninstall animation and remove segments with it
	cleaninstallAnimation.trigger(segments);
}

/**
 * @brief Increases the score by one
 */
void Curver::increaseScore()
{
	++roundScore;
	++totalScore;
}

/**
 * @brief Resets the round
 */
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
	headVisible = true;
}

/**
 * @brief Changes the alive state
 * @param alive Whether this Curver is alive
 */
void Curver::setAlive(const bool alive)
{
	if (isAlive() && !alive) {
		die();
	} else {
		this->alive = alive;
	}
}

/**
 * @brief Determines if the Curver is alive
 * @return \c True, iif alive
 */
bool Curver::isAlive() const
{
	return alive;
}

/**
 * @brief Appends a point to the Curver
 * @param pos The point to append
 * @param changingSegment Whether the Curver is changing segments at the moment
 */
void Curver::appendPoint(const QPointF pos, const bool changingSegment)
{
	// segments.size() test fixes a bug, where a client would crash due to network lag
	if (!changingSegment && (oldChangingSegment || segments.size() == 0)) {
		segments.push_back(std::make_unique<Segment>(parentNode, &material, thickness));
	}
	if (headVisible) {
		headNode->setPosition(pos);
	}
	if (!changingSegment && pos != lastPos) {
		const QPointF diff = pos - lastPos;
		const double length = sqrt(QPointF::dotProduct(diff, diff));
		float angle = acos(diff.x() / length);
		if (diff.y() < 0) {
			angle = -1 * angle;
		}
		Q_ASSERT(segments.size() > 0);
		segments.back()->appendPoint(pos, angle);
	}
	oldChangingSegment = changingSegment;
	lastPos = pos;
}

/**
 * @brief Prepares a segment event.
 *
 * Immediately forces the Curver to adapt the \a changingSegment value and plans a segment spawn at a random time from \a lower t0 \a upper.
 * @param changingSegment Whether the Curver should change segments right now
 * @param lower The lower random boundary of the segment event
 * @param upper The upper random boundary of the segment event
 */
void Curver::prepareSegmentEvent(bool changingSegment, int lower, int upper)
{
	nextSegmentEvent = QTime::currentTime().addMSecs(Util::randInt(lower, upper));
	this->changingSegment = changingSegment;
}

/**
 * @brief Updates the Curver assuming that \a deltat has gone by
 * @param deltat The amount of time since the last update in milliseconds
 * @param curvers All curvers
 */
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
	if (rotation == Rotation::ROTATE_LEFT) {
		rotate(-deltat * rotateVelocity);
	} else if (rotation == Rotation::ROTATE_RIGHT) {
		rotate(deltat * rotateVelocity);
	}
	lastPos += deltat * static_cast<double>(velocity) * direction;
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

/**
 * @brief Rotates the Curver
 * @param radians The amount of radian to rotate
 */
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

/**
 * @brief Triggers the death of the Curver
 *
 * Spawns an Explosion at the current location.
 */
void Curver::die()
{
	alive = false;
	(void) new Explosion(lastPos, parentNode, &material, this);
	emit died();
}

/**
 * @brief Compares two Curver objects
 * @param l The left Curver
 * @param r The right Curver
 * @return \c True, iif \a l is less than \a r, determined by whether the total score is less.
 */
bool operator <(const std::unique_ptr<Curver> &l, const std::unique_ptr<Curver> &r)
{
	return l->totalScore < r->totalScore;
}
