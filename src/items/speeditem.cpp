#include "speeditem.hpp"

/**
 * @brief Constructs the SpeedItem
 * @param parentNode The parent node in the scene graph
 * @param iconName The icon name
 * @param allowedUsers The allowed users
 * @param pos The location
 */
SpeedItem::SpeedItem(QSGNode *parentNode, QString iconName, AllowedUsers allowedUsers, QPointF pos) : Item(parentNode, iconName, allowedUsers, pos)
{
	activatedTime = 2000;
}

/**
 * @brief Increases the speed of the Curver
 * @param curver The affected Curver
 */
void SpeedItem::use(Curver *curver)
{
	curver->velocity *= 2;
}

/**
 * @brief Decreases the speed of the Curver
 * @param curver The affected Curver
 */
void SpeedItem::unUse(Curver *curver)
{
	curver->velocity /= 2;
}
