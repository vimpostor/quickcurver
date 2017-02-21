#ifndef CURVEITEM_H
#define CURVEITEM_H
#include <QPointF>
#include <QtQuick/qsgnode.h>
#include <QtQuick/qsgtexturematerial.h>
#include <QObject>
#include "segment.h"
#include <QQuickWindow>
#include <QOpenGLTexture>
#include <QQuickWindow>
#include <QtMath>
#include "qcurver.h"
#include "segment.h"
#include <QTimer>
#include <QImage>
#include <QQuickView>

#define SIZE 12
#define ITEMREDCOLOR QColor(245, 0, 87)
#define ITEMGREENCOLOR QColor(118, 255, 3)
#define ITEMBLUECOLOR QColor(61, 90, 254)

class CurveItem : public QObject
{
	Q_OBJECT
public:
    explicit CurveItem(QSGNode* node, QQuickView *view, int fieldsize, QString iconPath, bool redAllowed, bool greenAllowed, bool blueAllowed, QPointF *pos = NULL);
	~CurveItem();
	int getSize();
	QPointF getPos();
	bool testCollision(QPointF p);
	QColor getColor();
	void useItem(int playerCount, QCurver** curver, QCurver* collector);
	void setRound(int round);
    QString getIconName();
public slots:
//	void renderUseless();
protected slots:
	void deuseMyself();
	void deuseAll();
	void deuseOthers();

protected:
	virtual void use(QCurver* curver);
	virtual void deuse(QCurver* curver);
	int deUseTime = 0; //millisecs, 0 if no deUse at all
	void deuseIn();
	void useMyself();
	void useAll();
	void useOthers();
	int playerCount;
	QCurver **curver;
	QCurver *collector;
	QPointF pos;
	QSGNode* node;
    QSGTextureMaterial* material;
	QSGGeometry* geometry;
	QSGGeometryNode* gnode;
    QSGGeometry::TexturedPoint2D *vertices;
	QSGTexture* texture;
	QColor color;
	QTimer* timer;
	bool invisible = false;
	int round = -1;
};

#endif // CURVEITEM_H
