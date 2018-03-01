#include "agileitem.h"

/**
 * @brief Constructs the AgileItem
 * @param parentNode The parent node in the scene graph
 * @param iconName The icon name
 * @param allowedUsers The allowed users
 * @param pos The location
 */
AgileItem::AgileItem(QSGNode *parentNode, QString iconName, AllowedUsers allowedUsers, QPointF pos) : Item(parentNode, iconName, allowedUsers, pos)
{
	activatedTime = 3000;
}

/**
 * @brief Increases the rotational velocity of the given Curver
 * @param curver The Curver to make rotate faster
 */
void AgileItem::use(Curver *curver)
{
	curver->rotateVelocity *= 1.25;
}

/**
 * @brief Decreases the rotational velocity of the given Curver
 * @param curver The Curver to make rotate slower
 */
void AgileItem::unUse(Curver *curver)
{
	curver->rotateVelocity /= 1.25;
}
