#pragma once

#include "item.hpp"

/**
 * @brief Decreases the velocity of a Curver
 */
class SlowItem : public Item {
public:
	SlowItem(QSGNode *parentNode, QString iconName, AllowedUsers allowedUsers, QPointF pos, QQuickWindow *win);
private:
	virtual void use(Curver *curver) override;
	virtual void unUse(Curver *curver) override;
};
