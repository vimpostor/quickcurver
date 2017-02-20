#include "cleaninstallitem.h"

CleaninstallItem::CleaninstallItem(QSGNode *node, QQuickView *view, int fieldsize) : CurveItem(node, view, fieldsize, CLEANINSTALLICON, false, false, true) {
}

void CleaninstallItem::use(QCurver *curver) {
	curver->cleanInstall();
}
