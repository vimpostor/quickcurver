#pragma once

#include "item.hpp"

/**
 * @brief An Item that makes a Curver turn around faster
 */
class AgileItem : public Item {
public:
	AgileItem(QSGNode *parentNode, QString iconName, AllowedUsers allowedUsers, QPointF pos, QQuickWindow *win);
private:
	virtual void use(Curver *curver) override;
	virtual void unUse(Curver *curver) override;
};
