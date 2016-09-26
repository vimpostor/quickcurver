#include "fastitem.h"

FastItem::FastItem(QSGNode *node) : CurveItem(node) {
//	node->markDirty(QSGNode::DirtyGeometry);
	deUseTime = 2000;
}

void FastItem::use(QCurver *curver) {
	curver->doubleSpeed();
}

void FastItem::deuse(QCurver *curver) {
	curver->halfSpeed();
}
