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

/**
 * @brief Sets the parent node in the scene graph
 * @param parentNode The new parent node
 */
void Wall::setParentNode(QSGNode *parentNode)
{
	parentNode->appendChildNode(&geoNode);
}

/**
 * @brief Updates the dimension of the game
 */
void Wall::updateDimension()
{
	this->dimension = Settings::getSingleton().getDimension();
	resize();
}

/**
 * @brief Resizes the Wall according to the game's dimension
 */
void Wall::resize()
{
	QSGGeometry::Point2D *vertices = geometry.vertexDataAsPoint2D();
	vertices[0].set(0, 0);
	vertices[1].set(dimension.x(), 0);
	vertices[2].set(dimension.x(), dimension.y());
	vertices[3].set(0, dimension.y());
	geoNode.markDirty(QSGNode::DirtyGeometry);
}
