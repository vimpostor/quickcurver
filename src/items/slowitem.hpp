#ifndef SLOWITEM_H
#define SLOWITEM_H

#include "item.h"

/**
 * @brief Decreases the velocity of a Curver
 */
class SlowItem : public Item
{
public:
	SlowItem(QSGNode *parentNode, QString iconName, AllowedUsers allowedUsers, QPointF pos);
private:
	virtual void use(Curver *curver) override;
	virtual void unUse(Curver *curver) override;
};

#endif // SLOWITEM_H
