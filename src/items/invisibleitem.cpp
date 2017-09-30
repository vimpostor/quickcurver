#include "invisibleitem.h"

#define INVISIBLE_TIME 3000

InvisibleItem::InvisibleItem(QSGNode *parentNode, QString iconName, AllowedUsers allowedUsers, QPointF pos) : Item(parentNode, iconName, allowedUsers, pos)
{
}

void InvisibleItem::use(Curver *curver)
{
	curver->prepareSegmentEvent(true, INVISIBLE_TIME, INVISIBLE_TIME);
}
