#include "flashitem.h"

#define RECOVER_TIME 360

FlashItem::FlashItem(QSGNode *parentNode, QString iconName, AllowedUsers allowedUsers, QPointF pos) : Item(parentNode, iconName, allowedUsers, pos)
{
}

void FlashItem::use(Curver *curver)
{
	curver->prepareSegmentEvent(true, RECOVER_TIME, RECOVER_TIME);
	Curver::Rotation backup = curver->rotation;
	curver->rotation = Curver::ROTATE_NONE;
	curver->progress(RECOVER_TIME, PlayerModel::getSingleton().getCurvers());
	curver->rotation = backup;
}
