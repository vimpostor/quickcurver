#ifndef WALL_H
#define WALL_H

#include <QSGGeometryNode>
#include <QSGFlatColorMaterial>

#include "util.h"
#include "settings.h"

class Wall
{
public:
	explicit Wall();
	void setParentNode(QSGNode *parentNode);
	void updateDimension();
private:
	void resize();

	QPoint dimension {20, 20};
	QSGGeometryNode geoNode;
	QSGGeometry geometry = QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
	QSGFlatColorMaterial material;
};

#endif // WALL_H
