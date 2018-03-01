#ifndef AGILEITEM_H
#define AGILEITEM_H

#include "item.h"

/**
 * @brief An Item that makes a Curver turn around faster
 */
class AgileItem : public Item
{
public:
	AgileItem(QSGNode *parentNode, QString iconName, AllowedUsers allowedUsers, QPointF pos);
private:
	virtual void use(Curver *curver) override;
	virtual void unUse(Curver *curver) override;
};

#endif // AGILEITEM_H
