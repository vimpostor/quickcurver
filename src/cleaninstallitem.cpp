#include "cleaninstallitem.h"

CleaninstallItem::CleaninstallItem(QSGNode *node, int fieldsize) : CurveItem(node, fieldsize, false, false, true) {
}

void CleaninstallItem::use(QCurver *curver) {
	curver->cleanInstall();
}
