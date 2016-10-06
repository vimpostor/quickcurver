#ifndef FATTERITEM_H
#define FATTERITEM_H
#include "curveitem.h"
#include "qcurver.h"

class FatterItem : public CurveItem
{
public:
	FatterItem(QSGNode *node);
private:
	void use(QCurver *curver);
	void deuse(QCurver *curver);
};

#endif // FATTERITEM_H
