#include "speeditem.h"

SpeedItem::SpeedItem(QSGNode *parentNode, QString iconName, AllowedUsers allowedUsers, QPointF pos) : Item(parentNode, iconName, allowedUsers, pos)
{
	activatedTime = 2000;
}

void SpeedItem::use(Curver *curver)
{
	curver->velocity *= 2;
}

void SpeedItem::unUse(Curver *curver)
{
	curver->velocity /= 2;
}
