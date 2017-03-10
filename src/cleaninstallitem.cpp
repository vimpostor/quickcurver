#include "cleaninstallitem.h"

#define CLEANINSTALLICON ":/icons/communication/clear_all.svg"
#define REDALLOWED false
#define GREENALLOWED false
#define BLUEALLOWED true

CleaninstallItem::CleaninstallItem(QSGNode *node, QQuickView *view, int fieldsize, Server *server) : CurveItem(node, view, fieldsize, CLEANINSTALLICON, REDALLOWED, GREENALLOWED, BLUEALLOWED) {
	this->server = server;
}

void CleaninstallItem::use(QCurver *curver) {
	curver->cleanInstall();
	server->cleanInstall();
}

QString CleaninstallItem::getIconName() {
	return CLEANINSTALLICON;
}
