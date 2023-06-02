#include "slowitem.hpp"

/**
 * @brief Constructs the SlowItem
 * @param parentNode The parent node in the scene graph
 * @param iconName The icon name
 * @param allowedUsers The allowed users
 * @param pos The location
 * @param win The window to render in
 */
SlowItem::SlowItem(QSGNode *parentNode, QString iconName, AllowedUsers allowedUsers, QPointF pos, QQuickWindow *win)
	: Item(parentNode, iconName, allowedUsers, pos, win) {
	activatedTime = 2000;
}

/**
 * @brief Decreases the velocity of the Curver
 * @param curver The affected Curver
 */
void SlowItem::use(Curver *curver) {
	curver->velocity /= 2;
}

/**
 * @brief Increases the velocity of the Curver
 * @param curver The affected Curver
 */
void SlowItem::unUse(Curver *curver) {
	curver->velocity *= 2;
}
