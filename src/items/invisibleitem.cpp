#include "invisibleitem.hpp"

#define INVISIBLE_TIME 3000

/**
 * @brief Constructs the InvisibleItem
 * @param parentNode The parent node in the scene graph
 * @param iconName The icon name
 * @param allowedUsers The allowed users
 * @param pos The location
 * @param win The window to render in
 */
InvisibleItem::InvisibleItem(QSGNode *parentNode, QString iconName, AllowedUsers allowedUsers, QPointF pos, QQuickWindow *win)
	: Item(parentNode, iconName, allowedUsers, pos, win) {
}

/**
 * @brief Forces the Curver to immediately change segments
 * @param curver The affected Curver
 */
void InvisibleItem::use(Curver *curver) {
	curver->prepareSegmentEvent(true, INVISIBLE_TIME, INVISIBLE_TIME);
}
