#include "wall.h"

#define WALL_SIZE 4

Wall::Wall()
{
	geometry.setLineWidth(WALL_SIZE);
	geometry.setDrawingMode(GL_LINE_LOOP);
	geoNode.setGeometry(&geometry);
	material.setColor(Util::getColor("Cyan"));
	geoNode.setMaterial(&material);
	geometry.allocate(4);
	resize();
}

void Wall::setParentNode(QSGNode *parentNode)
{
	parentNode->appendChildNode(&geoNode);
}

void Wall::updateDimension()
{
	this->dimension = Settings::getSingleton().getDimension();
	resize();
}

void Wall::resize()
{
	QSGGeometry::Point2D *vertices = geometry.vertexDataAsPoint2D();
	vertices[0].set(0, 0);
	vertices[1].set(dimension.x(), 0);
	vertices[2].set(dimension.x(), dimension.y());
	vertices[3].set(0, dimension.y());
	geoNode.markDirty(QSGNode::DirtyGeometry);
}
