#include "fastitem.h"

FastItem::FastItem(QSGNode *node, int fieldsize) : CurveItem(node, fieldsize, true, false, false) {
//	node->markDirty(QSGNode::DirtyGeometry);
	deUseTime = 2000;
}

void FastItem::use(QCurver *curver) {
	curver->doubleSpeed();
}

void FastItem::deuse(QCurver *curver) {
	curver->halfSpeed();
}
