#include "bot.hpp"

#define LOOK_AHEAD 1000
#define MAX_ANGLE M_PI / 4
#define ANGLE_STEP M_PI / 24
#define WALL_MARGIN 100

/**
 * @brief Asks the Bot to make a move for a given Curver
 * @param c The Curver to make a move for
 */
void Bot::makeMove(Curver &c) {
	// TODO: Use checkForIntersection to precompute future positions of other curvers and take them into account
	// Otherwise bots "race" with other curvers heads right next to them
	// good default value, if we don't want to change anything, we can just return
	c.rotation = Curver::Rotation::ROTATE_NONE;
	auto &curvers = PlayerModel::get()->getCurvers();
	QPointF p = c.getPos();
	QPointF dir = c.getDirection();
	const QPointF dim = Settings::getSingleton().getDimension();
	const float v = c.velocity;
	const bool aboutToCollide = c.checkForIntersection(curvers, p, p + LOOK_AHEAD * v * dir);
	// are we going straight for a wall?
	if (p.x() < WALL_MARGIN || p.y() < WALL_MARGIN ||
		p.x() > dim.x() - WALL_MARGIN || p.y() > dim.y() - WALL_MARGIN) {
		// choose the shorter rotation away from the wall
		if (QPointF::dotProduct(p, dim / 2) > 0) {
			c.rotation = Curver::Rotation::ROTATE_LEFT;
		} else {
			c.rotation = Curver::Rotation::ROTATE_RIGHT;
		}
	}
	// else find a way to dodge
	const float angle = c.getAngle();
	float angleOffset = ANGLE_STEP / 2;
	float leftAngleDanger = 0, rightAngleDanger = 0;
	// find out how far danger is in both directions
	while ((leftAngleDanger * rightAngleDanger == 0) && angleOffset < MAX_ANGLE) {
		if (leftAngleDanger == 0) {
			const auto leftAngle = angle - angleOffset;
			if (c.checkForIntersection(curvers, p, p + LOOK_AHEAD * v * QPointF(cos(leftAngle), sin(leftAngle)))) {
				leftAngleDanger = angleOffset;
			}
		}
		if (rightAngleDanger == 0) {
			const auto rightAngle = angle + angleOffset;
			if (c.checkForIntersection(curvers, p, p + LOOK_AHEAD * v * QPointF(cos(rightAngle), sin(rightAngle)))) {
				rightAngleDanger = angleOffset;
			}
		}
		angleOffset += ANGLE_STEP;
	}
	// if no danger is found, set danger distance to something huge
	if (leftAngleDanger == 0) {
		leftAngleDanger = MAX_ANGLE;
	}
	if (rightAngleDanger == 0) {
		rightAngleDanger = MAX_ANGLE;
	}
	const float minDangerAngle = std::min(leftAngleDanger, rightAngleDanger);
	// only do something if danger REALLY is imminent
	if (aboutToCollide || (minDangerAngle < MAX_ANGLE)) {
		// choose the direction with danger the farthest away
		if (leftAngleDanger > rightAngleDanger) {
			c.rotation = Curver::Rotation::ROTATE_LEFT;
		} else {
			c.rotation = Curver::Rotation::ROTATE_RIGHT;
		}
	}
}
