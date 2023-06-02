#include "cleaninstallitem.hpp"

/**
 * @brief Constructs the CleanInstallItem
 * @param parentNode The parent node in the scene graph
 * @param iconName The icon name
 * @param allowedUsers The allowed users
 * @param pos The location
 * @param win The window to render in
 */
CleanInstallItem::CleanInstallItem(QSGNode *parentNode, QString iconName, Item::AllowedUsers allowedUsers, QPointF pos, QQuickWindow *win)
	: Item(parentNode, iconName, allowedUsers, pos, win) {
}

/**
 * @brief Cleaninstalls the Curver
 * @param curver The Curver to cleaninstall
 */
void CleanInstallItem::use(Curver *curver) {
	curver->cleanInstall();
}
