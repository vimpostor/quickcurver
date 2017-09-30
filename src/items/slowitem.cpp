#include "slowitem.h"

SlowItem::SlowItem(QSGNode *parentNode, QString iconName, AllowedUsers allowedUsers, QPointF pos) : Item(parentNode, iconName, allowedUsers, pos)
{
	activatedTime = 2000;
}

void SlowItem::use(Curver *curver)
{
	curver->velocity /= 2;
}

void SlowItem::unUse(Curver *curver)
{
	curver->velocity *= 2;
}
