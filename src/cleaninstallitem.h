#ifndef CLEANINSTALLITEM_H
#define CLEANINSTALLITEM_H
#include "curveitem.h"
#include <QSGNode>
#include "qcurver.h"
#include <QQuickView>

class CleaninstallItem : public CurveItem {
public:
    CleaninstallItem(QSGNode *node, QQuickView *view, int fieldsize);
    QString getIconName();
private:
	void use(QCurver *curver);
};

#endif // CLEANINSTALLITEM_H
