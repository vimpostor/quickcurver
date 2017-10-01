#ifndef GHOSTITEM_H
#define GHOSTITEM_H

#include "item.h"

class GhostItem : public Item
{
public:
	GhostItem(QSGNode *parentNode, QString iconName, AllowedUsers allowedUsers, QPointF pos);
private:
	virtual void use(Curver *curver) override;
	virtual void unUse(Curver *curver) override;
};

#endif // GHOSTITEM_H
