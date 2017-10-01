#include "ghostitem.h"

#define GHOST_TIME 3000

GhostItem::GhostItem(QSGNode *parentNode, QString iconName, AllowedUsers allowedUsers, QPointF pos) : Item(parentNode, iconName, allowedUsers, pos)
{
	activatedTime = GHOST_TIME;
}

void GhostItem::use(Curver *curver)
{
	curver->prepareSegmentEvent(true, GHOST_TIME, GHOST_TIME);
	curver->headVisible = false;
}

void GhostItem::unUse(Curver *curver)
{
	curver->headVisible = true;
}
