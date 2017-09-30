#ifndef INVISIBLEITEM_H
#define INVISIBLEITEM_H

#include "item.h"

class InvisibleItem : public Item
{
public:
	InvisibleItem(QSGNode *parentNode, QString iconName, AllowedUsers allowedUsers, QPointF pos);
private:
	virtual void use(Curver *curver) override;
};

#endif // INVISIBLEITEM_H
