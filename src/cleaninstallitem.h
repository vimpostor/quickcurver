#ifndef CLEANINSTALLITEM_H
#define CLEANINSTALLITEM_H
#include "curveitem.h"
#include <QSGNode>
#include "qcurver.h"
#include <QQuickView>
#include "server.h"

class CleaninstallItem : public CurveItem {
public:
    CleaninstallItem(QSGNode *node, QQuickView *view, int fieldsize, Server *server);
    QString getIconName();
private:
	void use(QCurver *curver);
    Server *server;
};

#endif // CLEANINSTALLITEM_H
