#ifndef INVISIBLEITEM_H
#define INVISIBLEITEM_H

#include "curveitem.h"

class InvisibleItem : public CurveItem {
public:
	InvisibleItem(QSGNode *node, QQuickView *view, int fieldsize);
	QString getIconName();
private:
	void use(QCurver *curver);
};

#endif // INVISIBLEITEM_H
