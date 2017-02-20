#ifndef FATTERITEM_H
#define FATTERITEM_H
#include "curveitem.h"
#include "qcurver.h"

#define FATTERICON ":/icons/action/arrow_expand.svg"

class FatterItem : public CurveItem
{
public:
    FatterItem(QSGNode *node, QQuickView *view, int fieldsize);
private:
	void use(QCurver *curver);
	void deuse(QCurver *curver);
};

#endif // FATTERITEM_H
