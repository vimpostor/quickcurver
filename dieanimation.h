#ifndef DIEANIMATION_H
#define DIEANIMATION_H
#include <QPointF>
#include <QObject>
#include <QtQuick/qsgnode.h>
#include <QtQuick/qsgflatcolormaterial.h>
#include <QtQuick/QSGOpaqueTextureMaterial>
#include <QTimer>
#include <QTime>
#include "segment.h"
#define PARTICLECOUNT 32
#define PARTICLESIZE 4
#define PARTICLEVELOCITY 512
#define PARTICLELIFETIME 512 //milliseconds

class dieAnimation : public QObject
{
	Q_OBJECT
public:
	explicit dieAnimation(QPointF p, QSGNode* node, QSGMaterial* material, QObject *parent = 0);

signals:
private:
	QSGNode* node;
	QSGOpacityNode* onode;
	QSGGeometryNode* gnode;
	QSGGeometry* geometry;
	QTimer* timer;
	QTime lastTime;
	void progress(float deltat, float timeSinceStart);
	QTime initialTime;
	QSGGeometry::Point2D *vertices;
	QPointF directions[PARTICLECOUNT];

private slots:
	void progress();
};

#endif // DIEANIMATION_H
