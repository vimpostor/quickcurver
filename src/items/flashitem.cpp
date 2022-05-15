#include "flashitem.hpp"

#define RECOVER_TIME 360

/**
 * @brief Constructs the FlashItem
 * @param parentNode The parent node in the scene graph
 * @param iconName The icon name
 * @param allowedUsers The allowed users
 * @param pos The location
 */
FlashItem::FlashItem(QSGNode *parentNode, QString iconName, AllowedUsers allowedUsers, QPointF pos) : Item(parentNode, iconName, allowedUsers, pos)
{
}

/**
 * @brief Flashes the given Curver forward a bit
 * @param curver The Curver to flash
 */
void FlashItem::use(Curver *curver)
{
	curver->prepareSegmentEvent(true, RECOVER_TIME, RECOVER_TIME);
	Curver::Rotation backup = curver->rotation;
	curver->rotation = Curver::Rotation::ROTATE_NONE;
	curver->progress(RECOVER_TIME, PlayerModel::getSingleton().getCurvers());
	curver->rotation = backup;
}
