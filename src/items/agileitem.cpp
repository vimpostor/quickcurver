#include "agileitem.h"

AgileItem::AgileItem(QSGNode *parentNode, QString iconName, AllowedUsers allowedUsers, QPointF pos) : Item(parentNode, iconName, allowedUsers, pos)
{
	activatedTime = 3000;
}

void AgileItem::use(Curver *curver)
{
	curver->rotateVelocity *= 1.25;
}

void AgileItem::unUse(Curver *curver)
{
	curver->rotateVelocity /= 1.25;
}
