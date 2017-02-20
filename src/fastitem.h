#ifndef FASTITEM_H
#define FASTITEM_H
#include "curveitem.h"
#include <QSGNode>
#include "qcurver.h"
#include <QQuickView>

#define FASTERICON ":/icons/action/clock_fast.svg"

class FastItem : public CurveItem
{
public:
    FastItem(QSGNode* node, QQuickView *view, int fieldsize);
private:
	void use(QCurver *curver);
	void deuse(QCurver *curver);
};

#endif // FASTITEM_H
