#include "invisibleitem.h"

#define INVISIBLEICON ":/Fluid/Controls/action/visibility_off.svg"
#define REDALLOWED false
#define GREENALLOWED true
#define BLUEALLOWED false

InvisibleItem::InvisibleItem(QSGNode *node, QQuickView *view, int fieldsize) : CurveItem(node, view, fieldsize, INVISIBLEICON, REDALLOWED, GREENALLOWED, BLUEALLOWED) {
}

void InvisibleItem::use(QCurver *curver) {
	curver->goInvisible(4000);
}

QString InvisibleItem::getIconName() {
	return INVISIBLEICON;
}
