#pragma once

#include "item.hpp"

/**
 * @brief Cleaninstalls the entire round
 *
 * Erases every line
 */
class CleanInstallItem : public Item {
public:
	CleanInstallItem(QSGNode *parentNode, QString iconName, AllowedUsers allowedUsers, QPointF pos, QQuickWindow *win);
private:
	virtual void use(Curver *curver) override;
};
