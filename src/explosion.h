#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <QObject>
#include <QtQuick>
#include <memory>
#include <QTimer>
#include <QPointF>

#include "util.h"

#define PARTICLECOUNT 64
#define PARTICLESIZE 4
#define PARTICLERANGE 256
#define PARTICLELIFETIME 512

class Explosion : public QObject
{
	Q_OBJECT
public:
	explicit Explosion(QPointF location, QSGNode *parentNode, QSGFlatColorMaterial *material, QObject *parent = nullptr);
	~Explosion();
private slots:
	void progress();
private:
	QPointF location;
	QSGNode *parentNode;
	std::unique_ptr<QSGOpacityNode> opacityNode;
	std::unique_ptr<QSGGeometryNode> geoNode;
	QSGGeometry geometry = QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
	QTimer timer;
	QTime initialTime = QTime::currentTime();
	QSGGeometry::Point2D *vertices;
	QPointF particleDirections[PARTICLECOUNT];
};

#endif // EXPLOSION_H
