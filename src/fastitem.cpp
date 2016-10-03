#include "fastitem.h"

FastItem::FastItem(QSGNode *node) : CurveItem(node, true, true, false) {
//	node->markDirty(QSGNode::DirtyGeometry);
	deUseTime = 2000;
}

void FastItem::use(QCurver *curver) {
	curver->doubleSpeed();
}

void FastItem::deuse(QCurver *curver) {
	curver->halfSpeed();
}
