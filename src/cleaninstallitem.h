#ifndef CLEANINSTALLITEM_H
#define CLEANINSTALLITEM_H
#include "curveitem.h"
#include <QSGNode>
#include "qcurver.h"
#include <QQuickView>

class CleaninstallItem : public CurveItem
{
public:
    CleaninstallItem(QSGNode *node, int fieldsize);
private:
	void use(QCurver *curver);
};

#endif // CLEANINSTALLITEM_H
