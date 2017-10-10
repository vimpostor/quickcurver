#include "bot.h"

#define LOOK_AHEAD 2000
#define MAX_ANGLE M_PI/2
#define ANGLE_STEP 0.125
#define WALL_MARGIN 100

/**
 * @brief Asks the Bot to make a move for a given Curver
 * @param c The Curver to make a move for
 */
void Bot::makeMove(Curver *c)
{
	// good default value, if we don't want to change anything, we can just return
	c->rotation = Curver::Rotation::ROTATE_NONE;
	auto &curvers = PlayerModel::getSingleton().getCurvers();
	QPointF a = c->getPos();
	QPointF dir = c->getDirection();
	const QPointF dim = Settings::getSingleton().getDimension();
	const float v = c->velocity;
	if (!c->checkForIntersection(curvers, a, a + LOOK_AHEAD * v * dir)) {
		if (a.x() < WALL_MARGIN || a.y() < WALL_MARGIN || \
				a.x() > dim.x() - WALL_MARGIN || a.y() > dim.y() - WALL_MARGIN) {
			if (QPointF::dotProduct(a, dim / 2) > 0) {
				c->rotation = Curver::Rotation::ROTATE_LEFT;
			} else {
				c->rotation = Curver::Rotation::ROTATE_RIGHT;
			}
		}
		return;
	}
	// else find a way to dodge
	const float angle = c->getAngle();
	float leftAngle, rightAngle;
	for (leftAngle = angle, rightAngle = angle; (rightAngle - angle) < MAX_ANGLE; leftAngle -= ANGLE_STEP, rightAngle += ANGLE_STEP) {
		if (!c->checkForIntersection(curvers, a, a + LOOK_AHEAD * v * QPointF(cos(leftAngle), sin(leftAngle)))) {
			c->rotation = Curver::Rotation::ROTATE_LEFT;
			return;
		}
		if (!c->checkForIntersection(curvers, a, a + LOOK_AHEAD * v * QPointF(cos(rightAngle), sin(rightAngle)))) {
			c->rotation = Curver::Rotation::ROTATE_RIGHT;
			return;
		}
	}
}
