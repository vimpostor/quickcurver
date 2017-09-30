#include "headnode.h"

HeadNode::HeadNode(QSGNode *parentNode, QSGFlatColorMaterial *material) : QSGGeometryNode ()
{
	this->parentNode = parentNode;

	geometry.setLineWidth(6);
	geometry.setDrawingMode(GL_POINTS);
	this->setGeometry(&geometry);
	this->setMaterial(material);
	geometry.allocate(1);
	parentNode->appendChildNode(this);
}

HeadNode::~HeadNode()
{
	parentNode->removeChildNode(this);
}

void HeadNode::setPosition(const QPointF newPos)
{
	this->pos = newPos;
	QSGGeometry::Point2D *vertices = geometry.vertexDataAsPoint2D();
	vertices[0].set(pos.x(), pos.y());
	this->markDirty(QSGNode::DirtyGeometry);
}
