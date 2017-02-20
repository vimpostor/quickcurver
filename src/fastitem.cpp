#include "fastitem.h"

FastItem::FastItem(QSGNode *node, QQuickView *view, int fieldsize) : CurveItem(node, view, fieldsize, FASTERICON, true, false, false) {
//	node->markDirty(QSGNode::DirtyGeometry);
	deUseTime = 2000;
}

void FastItem::use(QCurver *curver) {
	curver->doubleSpeed();
}

void FastItem::deuse(QCurver *curver) {
	curver->halfSpeed();
}
