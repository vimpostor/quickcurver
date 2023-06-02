#include "ghostitem.hpp"

#define GHOST_TIME 2000

/**
 * @brief Constructs the GhostItem
 * @param parentNode The parent node in the scene graph
 * @param iconName The icon name
 * @param allowedUsers The allowed users
 * @param pos The location
 * @param win The window to render in
 */
GhostItem::GhostItem(QSGNode *parentNode, QString iconName, AllowedUsers allowedUsers, QPointF pos, QQuickWindow *win)
	: Item(parentNode, iconName, allowedUsers, pos, win) {
	activatedTime = GHOST_TIME;
}

/**
 * @brief Renders the Curver invisible
 * @param curver The affected Curver
 */
void GhostItem::use(Curver *curver) {
	curver->prepareSegmentEvent(true, GHOST_TIME, GHOST_TIME);
	curver->headVisible = false;
}

/**
 * @brief Renders the Curver visible
 * @param curver The affected Curver
 */
void GhostItem::unUse(Curver *curver) {
	curver->headVisible = true;
}
