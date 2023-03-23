#include "headnode.hpp"

/**
 * @brief Constructs a HeadNode
 * @param parentNode The parent node in the scene graph
 * @param material The material to use for this node
 */
HeadNode::HeadNode(QSGNode *parentNode, QSGFlatColorMaterial *material)
	: QSGGeometryNode() {
	this->parentNode = parentNode;

	geometry.setDrawingMode(QSGGeometry::DrawTriangleStrip);
	this->setGeometry(&geometry);
	this->setMaterial(material);
	geometry.allocate(4);
	parentNode->appendChildNode(this);
}

HeadNode::~HeadNode() {
	parentNode->removeChildNode(this);
}

/**
 * @brief Updates the location
 * @param newPos The new location
 */
void HeadNode::setPosition(const QPointF newPos) {
	constexpr float pointSize = 2.f;
	this->pos = newPos;
	QSGGeometry::Point2D *vertices = geometry.vertexDataAsPoint2D();
	vertices[0].set(pos.x() - pointSize, pos.y() - pointSize);
	vertices[1].set(pos.x() - pointSize, pos.y() + pointSize);
	vertices[2].set(pos.x() + pointSize, pos.y() - pointSize);
	vertices[3].set(pos.x() + pointSize, pos.y() + pointSize);
	this->markDirty(QSGNode::DirtyGeometry);
}
