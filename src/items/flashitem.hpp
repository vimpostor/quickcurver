#pragma once

#include "item.hpp"

/**
 * @brief Flashes a Curver forward depending on its velocity
 */
class FlashItem : public Item {
public:
	FlashItem(QSGNode *parentNode, QString iconName, AllowedUsers allowedUsers, QPointF pos, QQuickWindow *win);
private:
	virtual void use(Curver *curver) override;
};
