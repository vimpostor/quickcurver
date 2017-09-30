#include "cleaninstallitem.h"

CleanInstallItem::CleanInstallItem(QSGNode *parentNode, QString iconName, Item::AllowedUsers allowedUsers, QPointF pos) : Item(parentNode, iconName, allowedUsers, pos)
{
}

void CleanInstallItem::use(Curver *curver)
{
	curver->cleanInstall();
}
