#include "headnode.h"

/**
 * @brief Constructs a HeadNode
 * @param parentNode The parent node in the scene graph
 * @param material The material to use for this node
 */
HeadNode::HeadNode(QSGNode *parentNode, QSGFlatColorMaterial *material) : QSGGeometryNode ()
{
	this->parentNode = parentNode;

	geometry.setLineWidth(6);
	geometry.setDrawingMode(QSGGeometry::DrawPoints);
	this->setGeometry(&geometry);
	this->setMaterial(material);
	geometry.allocate(1);
	parentNode->appendChildNode(this);
}

HeadNode::~HeadNode()
{
	parentNode->removeChildNode(this);
}

/**
 * @brief Updates the location
 * @param newPos The new location
 */
void HeadNode::setPosition(const QPointF newPos)
{
	this->pos = newPos;
	QSGGeometry::Point2D *vertices = geometry.vertexDataAsPoint2D();
	vertices[0].set(pos.x(), pos.y());
	this->markDirty(QSGNode::DirtyGeometry);
}
