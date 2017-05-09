#include "fastitem.h"

#define FASTERICON ":/Fluid/Controls/maps/directions_bike.svg"
#define REDALLOWED true
#define GREENALLOWED true
#define BLUEALLOWED true

FastItem::FastItem(QSGNode *node, QQuickView *view, int fieldsize) : CurveItem(node, view, fieldsize, FASTERICON, REDALLOWED, GREENALLOWED, BLUEALLOWED) {
//	node->markDirty(QSGNode::DirtyGeometry);
	deUseTime = 2000;
}

void FastItem::use(QCurver *curver) {
	curver->doubleSpeed();
}

void FastItem::deuse(QCurver *curver) {
	curver->halfSpeed();
}

QString FastItem::getIconName() {
	return FASTERICON;
}
