#ifndef FASTITEM_H
#define FASTITEM_H
#include "curveitem.h"
#include <QSGNode>
#include "qcurver.h"

class FastItem : public CurveItem
{
public:
	FastItem(QSGNode* node);
private:
	void use(QCurver *curver);
	void deuse(QCurver *curver);
};

#endif // FASTITEM_H
