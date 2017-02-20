#include "cleaninstallitem.h"

#define CLEANINSTALLICON ":/icons/communication/clear_all.svg"
#define REDALLOWED false
#define GREENALLOWED false
#define BLUEALLOWED true

CleaninstallItem::CleaninstallItem(QSGNode *node, QQuickView *view, int fieldsize) : CurveItem(node, view, fieldsize, CLEANINSTALLICON, REDALLOWED, GREENALLOWED, BLUEALLOWED) {
}

void CleaninstallItem::use(QCurver *curver) {
	curver->cleanInstall();
}
