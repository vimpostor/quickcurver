#pragma once

#include "item.hpp"

/**
 * @brief Increases the velocity of a Curver
 */
class SpeedItem : public Item {
public:
	SpeedItem(QSGNode *parentNode, QString iconName, AllowedUsers allowedUsers, QPointF pos, QQuickWindow *win);
private:
	virtual void use(Curver *curver) override;
	virtual void unUse(Curver *curver) override;
};
