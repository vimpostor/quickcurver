#pragma once

#include "item.hpp"

/**
 * @brief Renders a Curver invisible and makes it immune to collisions
 */
class InvisibleItem : public Item {
public:
	InvisibleItem(QSGNode *parentNode, QString iconName, AllowedUsers allowedUsers, QPointF pos, QQuickWindow *win);
private:
	virtual void use(Curver *curver) override;
};
