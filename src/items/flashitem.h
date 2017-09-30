#ifndef FLASHITEM_H
#define FLASHITEM_H

#include "item.h"

class FlashItem : public Item
{
public:
	FlashItem(QSGNode *parentNode, QString iconName, AllowedUsers allowedUsers, QPointF pos);
private:
	virtual void use(Curver *curver) override;
};

#endif // FLASHITEM_H
