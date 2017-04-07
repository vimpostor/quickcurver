#ifndef FASTITEM_H
#define FASTITEM_H
#include "curveitem.h"
#include <QSGNode>
#include <QQuickView>

class FastItem : public CurveItem
{
public:
	FastItem(QSGNode* node, QQuickView *view, int fieldsize);
	QString getIconName();
private:
	void use(QCurver *curver);
	void deuse(QCurver *curver);
};

#endif // FASTITEM_H
