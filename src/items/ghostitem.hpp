#ifndef GHOSTITEM_H
#define GHOSTITEM_H

#include "item.hpp"

/**
 * @brief Renders a Curver invisible while still being able to collide
 */
class GhostItem : public Item {
public:
	GhostItem(QSGNode *parentNode, QString iconName, AllowedUsers allowedUsers, QPointF pos, QQuickWindow *win);
private:
	virtual void use(Curver *curver) override;
	virtual void unUse(Curver *curver) override;
};

#endif // GHOSTITEM_H
