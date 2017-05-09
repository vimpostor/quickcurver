#include "fatteritem.h"

#define FATTERICON ":/Fluid/Controls/maps/local_dining.svg"
#define REDALLOWED true
#define GREENALLOWED false
#define BLUEALLOWED false

FatterItem::FatterItem(QSGNode *node, QQuickView *view, int fieldsize) : CurveItem(node, view, fieldsize, FATTERICON, REDALLOWED, GREENALLOWED, BLUEALLOWED) {
	deUseTime = 5000;
}

void FatterItem::use(QCurver *curver) {
	curver->doubleThickness();
}

void FatterItem::deuse(QCurver *curver) {
	curver->halfThickness();
}

QString FatterItem::getIconName() {
	return FATTERICON;
}
