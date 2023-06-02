#include "speeditem.hpp"

/**
 * @brief Constructs the SpeedItem
 * @param parentNode The parent node in the scene graph
 * @param iconName The icon name
 * @param allowedUsers The allowed users
 * @param pos The location
 * @param win The window to render in
 */
SpeedItem::SpeedItem(QSGNode *parentNode, QString iconName, AllowedUsers allowedUsers, QPointF pos, QQuickWindow *win)
	: Item(parentNode, iconName, allowedUsers, pos, win) {
	activatedTime = 2000;
}

/**
 * @brief Increases the speed of the Curver
 * @param curver The affected Curver
 */
void SpeedItem::use(Curver *curver) {
	curver->velocity *= 2;
}

/**
 * @brief Decreases the speed of the Curver
 * @param curver The affected Curver
 */
void SpeedItem::unUse(Curver *curver) {
	curver->velocity /= 2;
}
