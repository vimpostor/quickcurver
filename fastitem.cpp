#include "fastitem.h"

FastItem::FastItem(QSGNode *node) : CurveItem(node) {
	deUseTime = 2000;
}

void FastItem::use(QCurver *curver) {
	curver->doubleSpeed();
}

void FastItem::deuse(QCurver *curver) {
	curver->halfSpeed();
}
