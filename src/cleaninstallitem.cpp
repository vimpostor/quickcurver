#include "cleaninstallitem.h"

CleaninstallItem::CleaninstallItem(QSGNode *node) : CurveItem(node, false, false, true) {
}

void CleaninstallItem::use(QCurver *curver) {
	curver->cleanInstall();
}
